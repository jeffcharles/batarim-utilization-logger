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

* CMake (version 2.8 or higher)

Installing on Linux
-------------------

1. Untar the source archive, `tar -xvzf batarim.tar.gz` (or clone the repo)
2. Enter the project directory, `cd batarim`
3. Generate a UNIX makefile, `./cmake_scripts/generate.sh`
4. Enter the release build directory, `cd build/release`
5. Compile and install the application, `make install`, specify a value for the
    `DESTDIR` flag if you would like to change the installation directory
    prefix (not recommendend)
6. If you did not run `make install` as root, you will need to add the
    `DESTDIR` path with `/usr/local/lib` appended to your `LD_LIBRARY_PATH`
    environment variable or Batarim will not be able to locate a required
    shared library.
7. If there are any errors that occur after the files have been copied as part
    of `make install`, correct them and re-run `./install_scripts/setup.sh` at
    the root of the project directory (as root if you ran `make install` as
    root). 

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

Coding conventions
------------------

* Use spaces instead of tabs
* Each tabstop should be 4 spaces
* Header files should use guard definitions in the form of 
`GUARD_<header_name>_h`
* Typenames should use Pascal Case
* Other names should use lower-case letters separated by underscores
* Private and protected variables should be post-fixed with an underscore
* Class members should be declared in the order of: public, protected, private
