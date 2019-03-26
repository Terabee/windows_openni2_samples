#include <OpenNI.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>

using namespace std;
using namespace openni;
using namespace cv;

int main(int argc, char* argv[])
{
	if (STATUS_OK != OpenNI::initialize()) {
		cout << "After initialization: " << OpenNI::getExtendedError() << endl;
		return 1;
	}

	Device devDevice;
	if (STATUS_OK != devDevice.open(ANY_DEVICE)) {
		cout << "Cannot open device: " << OpenNI::getExtendedError() << endl;
		return 1;
	}

	VideoMode mode;
	VideoStream vsDepth;

	if (STATUS_OK != vsDepth.create(devDevice, SENSOR_DEPTH)) {
		cout << "Cannot create depth stream on device: " << OpenNI::getExtendedError() << endl;
		return 1;
	}
	else {
		mode = vsDepth.getVideoMode();
		cout << "depth video mode - FPS=" << mode.getFps() << ", X=" << mode.getResolutionX() << ", Y=" << mode.getResolutionY() << endl;
		vsDepth.start();
	}

	VideoFrameRef depth_frame;
	int max_dist = 5000;
	int min_dist = 0;
	bool reverse_colormap = true;

	if (argc == 3)
	{
		max_dist = atoi(argv[2]);
		min_dist = atoi(argv[1]);
	}
	cv::namedWindow("Depth View", CV_WINDOW_NORMAL);

	while (waitKey(1) == -1 && cv::getWindowProperty("Depth View", WND_PROP_AUTOSIZE) != -1) {
		if (vsDepth.isValid()) {
			if (STATUS_OK == vsDepth.readFrame(&depth_frame)) {
				Mat imgDepth(depth_frame.getHeight(), depth_frame.getWidth(), CV_16UC1, (void*)depth_frame.getData());
				Mat imgDepthScaled(depth_frame.getHeight(), depth_frame.getWidth(), CV_16UC1);
				Mat img8bitDepth;

				for (int i = 0; i < depth_frame.getHeight(); i++)
				{
					uint16_t * row = imgDepth.ptr<uint16_t>(i);
					uint16_t * rowScaled = imgDepthScaled.ptr<uint16_t>(i);
					for (int j = 0; j < depth_frame.getWidth(); j++)
					{
						uint16_t* pixel = &row[j];
						uint16_t* pixelScaled = &rowScaled[j];

						if (*pixel > max_dist) {
							*pixelScaled = max_dist;
						}
						else if (*pixel < min_dist && *pixel > 0) {
							*pixelScaled = min_dist;
						}
						else if (*pixel == 0)
						{
							*pixelScaled = max_dist;
						}
						else *pixelScaled = *pixel;

						if (reverse_colormap) {
							*pixelScaled = 65535 - (*pixelScaled - min_dist)*(int)(65535 / (max_dist - min_dist));
						}
						else *pixelScaled = (*pixelScaled - min_dist)*(int)(65535 / (max_dist - min_dist));
					}
				}

				// Convert 16bit to 8bit greyscale
				imgDepthScaled.convertTo(img8bitDepth, CV_8U, 1.0 / 257.0);

				// Apply color map to 8bit frame
				Mat rgb(60, 80, CV_8UC3);
				applyColorMap(img8bitDepth, rgb, COLORMAP_JET);

				// Set invalid pixel to black for any colormap
				for (int i = 0; i < depth_frame.getHeight(); i++) {
					uint16_t * row = imgDepth.ptr<uint16_t>(i);
					for (int j = 0; j < depth_frame.getWidth(); j++) {
						uint16_t* pixel = &row[j];
						if (*pixel == 0)
						{	
							Vec3b color = rgb.at<Vec3b>(Point(j, i));
							color[0] = 0;
							color[1] = 0;
							color[2] = 0;
							rgb.at<Vec3b>(Point(j, i)) = color;
						}
					}
				}

				cv::resize(rgb, rgb, cv::Size(), 10, 10, 1);
				imshow("Depth View", rgb);
			}
		}
	}
	vsDepth.destroy();

	devDevice.close();
	OpenNI::shutdown();

	return 0;
}