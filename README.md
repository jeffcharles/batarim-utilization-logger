Batarim Utilization Logger
==========================

Purpose
-------

This application will log various utilization statistics (e.g., overall CPU and 
RAM usage) into a CSV file along with what applications were using the most CPU
and RAM at a given time and what window was focused. This CSV file can then be
analyzed to determine if there are any problems with resource utilization
levels, if any applications are stressing the hardware, or for any other
purpose that you would like.

The application's installer automatically sets up the logger to run at 15
minute intervals and write out the entry to a log file. By default, the logger
places the log file in `C:\ProgramData\Batarim\batarim_<version>.csv` on
Windows and `/var/log/batarim_<version>.csv` on Linux.

How to Change the Logging Frequency
-------------------------------

On Windows, run `schtasks /delete /tn Batarim\Batarim`, then run
`schtasks /create /tn Batarim\Batarim /tr "\"<path_to_install>\task.vbs\"" /sc MINUTE /mo <new_logging_frequency>`.

On Linux, if you installed through a package manager or installed as root, open
up batarim's crontab by running `crontab -u batarim -e` with sudo privileges or
the root account and change the number after `*/` to the number of minutes that
you want it to wait between log entries.

On Linux, if you installed the package manually as a non-root user, open your
own crontab by running `crontab -e` and change the number after `*/` to the
number of minutes that you want it to wait between log entries.

Linux Security Notes
--------------------

On Linux, Batarim's installer tries to create a seperate system account for
Batarim (called batarim) that runs the logger and owns the log file. Since the
logger attempts to log what window you have focused, the Batarim user account
requires access to your X session so it will insert a line into your user's GDM
start-up script (~/.gnomerc) to grant it access. Removing that line will still
allow the application to run but it will not be able to determine what window
you have focused when the logger is run.

Linux Dependencies
------------------

* coreutils
* cron
* grep
* libc-bin
* libc6
* libstdc++6
* libx11-6
* procps
* sed
* x11-xserver-utils
* cmake, version 2.8 or higher (if compiling)
* gcc, version 4.5.2 or higher (if compiling)

Compiling and Installing on Linux
---------------------------------

1. Either clone the Git repo or download and untar the source archive,
    `tar -xvzf batarim.tar.gz`
2. Enter the project directory, `cd batarim`
3. Generate a UNIX makefile, `./cmake_scripts/generate.sh`
4. Enter the release build directory, `cd build/release`
5. Compile the application, `make`
6. If you have sudo privileges or are root and are on Ubuntu Linux or Linux
    Mint, create a Debian package with `make deb` and run
    `dpkg --install batarim_<version_number_and_arch>.deb`
7. If you do not have sudo privileges or are not root or are not on a Ubuntu
    Linux or Linux Mint, install the application with, `make install`, specify
    a value for the `DESTDIR` flag if you would like to change the installation
    directory prefix (not recommendend)
8. If you did not run `make install` as root, you will need to add the
    `DESTDIR` path with `/usr/local/lib` appended to your `LD_LIBRARY_PATH`
    environment variable or Batarim will not be able to locate a required
    shared library.
9. If there are any errors that occur after the files have been copied as part
    of `make install`, correct them and re-run `./install_scripts/setup.sh` at
    the root of the project directory (as root if you ran `make install` as
    root). 

Uninstalling on Linux (with a manual compile and install)
---------------------------------------------------------

1. Go to your downloaded version of the source archive and generate the UNIX
    makefile if necessary following the instructions above
2. Go to `build/release`
3. Run `make uninstall`
4. Note, there is currently a known issue where in some cases the files
    installed will not be deleted, in that case it is safe to manually remove
    the files as the teardown script run as part of the uninstall should have
    stopped the logging cronjob

Windows Dependencies for Compiling
----------------------------------

* CMake, version 2.8 or higher
* Microsoft Visual C++ 2010 or higher

How to Set Up a Build on Windows
--------------------------------

1. Clone the repository or download and unzip the source archive
2. Enter the `cmake_scripts` directory
3. Run `generate.bat`
4. Click `Configure` in the bottom left of the GUI
5. Select your preferred environment or build system (e.g., a version of Visual 
    Studio)
6. Click `Generate` which is next to `Configure`, once the configuration is done
7. Close the window once generation is done
8. Your Visual Studio solution file will be in the `build` directory under the
    root of the repository
9. If you wish to clear the build, you can either delete the `build` directory
    or run `clear.bat` under `cmake_scripts`
10. Building the solution will result in an MSI file being generated in
    `build\windows_installer` alongside all DLL and EXE files

Project Documentation (including information on contributing code)
------------------------------------------------------------------

For project documentation and information on contributing code, please take a
look inside the `docs` directory.
