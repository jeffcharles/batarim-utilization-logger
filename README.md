Batarim Utilization Logger
==========================

Purpose
-------

This application will log various utilization statistics (e.g., CPU, GPU, and 
RAM usage) into a CSV file along with what applications were active at the
time. This CSV file can then be analyzed to determine if there are any 
problems with resource utilization levels, if any applications are stressing 
the hardware, or for any other purpose that you would like.

Prerequisites
-------------

* CMake (version 2.6 or higher)

How to set up on Windows
------------------------

* Clone the repository
* Enter the `cmake_scripts` directory
* Run `generate.bat`
* Click `Configure` in the bottom left of the GUI
* Select your preferred environment or build system (e.g., a version of Visual 
Studio or NMake)
* Click `Generate` which is next to `Configure`, once the configuration is done
* Close the window once generation is done
* Your makefiles or Visual Studio project file will be in the `build` directory
under the root of the repository
* Source files will be under the `BatarimUL` project inside the solution file
if you are using Visual Studio
* If you wish to clear the build, you can either delete the `build` directory
or run `clear.bat` under`cmake_scripts`
