# README

The purpose of this README is to give guidelines for developing applications with Terabee 3Dcam 80x60 using the OpenNI 2 framework on Windows. 

For more information about the Terabee 3Dcam 80x60, please click [here](https://www.terabee.com/shop/3d-tof-cameras/terabee-3dcam/).

## System Requirements

* Windows 7 or later
* Visual Studio 2015

## Prepare software development environment

### Install OpenCV 2

In order to build and run samples from Terabee, you will have to install OpenCV.

Install OpenCV 3.4.1 pre-built binaries for Windows (available [here](https://opencv.org/releases.html)). The rest of the guide will assume that OpenCV in installed to C:\opencv.

The visual studio project provided in this repository is designed to work with Visual Studio 2015 (vc14). If you wan to use a newer version of OpenCV, you will have to check that pre-built binaries are compatible with your Visual Studio version and adapt include files, library path and files name accordingly.

### Install Terabee 3D depth camera SDK

* Download the Windows SDK from the Downloads section of Terabee 3Dcam [here](https://www.terabee.com/shop/3d-tof-cameras/terabee-3dcam/).

* Install the SDK by running the file relevant to your system architecture and desired OpenNI version.

## Start developing your application

To get started with your application development, we provide sample codes to help you initiate your development. The following steps provide instructions on how to download and run sample codes.

### Install Git

* Please download Git-SCM tools [here](https://git-scm.com/) to install Git client for Windows platform.

### Clone sample code
Clone the project using a git-bash console or the Git GUI:

```
git clone https://github.com/terabee/windows_openni2_samples.git Terabee_samples
```

### Build and Run Terabee samples

#### DepthViewer

This sample will show you how to access camera data and display it into a window. It was created using Visual Studio 2013. If you want to use another version, you will have make sure it is compatible with the installed OpenCV version.

To run the sample, please follow those steps:

* Launch Visual Studio IDE by clicking DepthViewer.vcxproj
* Click on the Run button to build and launch the sample

The project is setup to automatically copy runtime files after each succesful build. However, if this doesn't work here is how to proceed manually:
* Copy content in Redist folder to the folder where execution is. The Redist folder is C:\Program Files\OpenNI2\Redist or C:\Program Files (x86)\OpenNI2\Redist depends on your target platform.
* Copy Terabee driver modules DeviceModules2.dll and RvcLib.dll from C:\Program Files\TERABEEToF\x64\ (or C:\Program Files\TERABEEToF\x86\, depends on your target platform) to OpenNI2\Drivers

### Build your own application
Taking as example the sample we provide, you can start to develop your own application. What you need to know to create a new project is:

* After the new project has been created, you need to add OpenNI2 headers and library path in the project configuration property. That is,

1. Right click the project in Solution Explorer and choose Properties

2. Expand "VC++ Directories", add "C:\Program Files\OpenNI2\Include" to "Include Directories" and add "C:\Program Files\OpenNI2\Lib" to "Library Directories"

3. If you are developing a 32-bit application in a 64-bit machine, the paths might modified to "C:\Program Files (x86)\OpenNI2\Include" and "C:\Program Files (x86)\OpenNI2\Lib"

4. Then, expand "Linker" in "Configuration Properties", add "OpenNI2.lib" to "Input->Additional Dependencies". This is also where you would link additionnal OpenCV librairies.

5. Develop your application.

6. Before testing your application, copy the content in Redist folder to the execution folder. The Redist folder is located at C:\Program Files\OpenNI2\Redist or C:\Program Files (x86)\OpenNI2\Redist depending on your target platform.

7. Copy the Terabee driver modules DeviceModules2.dll and RvcLib.dll from C:\Program Files\TERABEEToFToF\x64\ (or C:\Program Files\TERABEEToFToF\x86\, depending on your target platform) to OpenNI2\Drivers
