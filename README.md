# Introduction

This project is a tiny graphics framework used in ...

It has missing functionality that you will need to implement.

# Prerequisites

This section describes ***what you need to do and install*** before actually building the code.

## Install a compiler

The code is written in C++11 and GLSL. The minimum versions of relevant compilers are:

-   Visual C++ 14.0 (part of Visual Studio 2015) or newer
-   GCC 5 or newer.

In either case, it is ***strongly*** recommended to build the code with the most recent available version. Older compiler versions may not fully respect the C++11 standard.

## Check your graphics capabilities

Graphics capabilities are decided by the combination of your computer's hardware, drivers, and operating system.

This project requires OpenGL ***version 3.3 core profile,*** or newer. If your computer has a processor manufactured within the last few years, you should be safe: a modern CPU has [graphics capabilities itself][ref-igp-wiki], see for example [Intel HD Graphics][ref-intelhd-wiki]. If you have a dedicated graphics card, you should be even safer.

***If you're not sure,*** follow the steps in this section to find out.

1.  Update your graphics drivers. Instructions on how to install or update drivers differ across manufacturers and are ***not*** covered here.
2.  Check the OpenGL version your device supports:

    -   Windows:
        1.  Go here: https://github.com/gkv311/wglinfo/releases/
        2.  Download the latest version of the file called `wglinfo64.exe` and save it to Desktop
        3.  Open a command prompt, and run:
            ```bat
            cd %userprofile%\Desktop
            wglinfo64.exe | findstr /c:"(core profile) version string"
            ```
        4.  Check the output, for example:
            ```
            [WGL] OpenGL (core profile) version string: 4.6.0 - Build 27.20.100.8681
            ```
        
    -   Linux, if installed directly on device, or in a virtual machine:
        1.  One of the following:
            -   Debian (Ubuntu): `sudo apt install -y mesa-utils`
            -   Red Hat (Fedora): `sudo dnf install -y glx-utils`
            -   Arch (x86_64): `sudo pacman -Sy mesa-demos`
        2.  `glxinfo | grep "core profile version string"`
        3.  Check the output, for example:
            ```
            OpenGL core profile version string: 3.3 (Core Profile) Mesa 20.0.8
            ```

    -   Linux, if installed via WSL2: if you have Windows and want to use (or develop) this project on Linux, but don't feel like using a virtual machine or installing Linux directly on your device, then you can use WSL2. This option is slightly more adventurous; for more details, ***TBD***.

## Install the build tools

This project uses [CMake][ref-cmake]. It a nutshell, CMake doesn't compile your code, instead it creates files that you then use to compile your code (for example, a Makefile on Linux, macOS, MinGW; a Visual Studio project on Windows; and so on).

The minimum compatible version is 3.16, however, as with the compilers, it is ***strongly*** recommended to use the latest version. To install CMake, follow these steps:

-   Windows:
    1.  Go to the CMake downloads page and use the installer

-   Linux:
    1.  Use your package manager to install `cmake`
    2.  Check the version using `cmake --version`
    3.  Depending on the version:
        -   If it's 3.16 or newer, you're all set
        -   Otherwise, run `./tools/install-cmake.sh && . ~/.profile` in a terminal

After installation, check that `cmake` is in your `PATH` environment variable (it should happen automatically); for this, run `cmake --version` again. Instructions on how to add an executable to your `PATH` differ across operating systems and are ***not*** covered here.

## Install the third-party libraries

There are some open-source libraries that this project uses. To install them:

-   Windows: you don't need to do anything - all necessary libraries are already provided with the code

-   Linux:
    1.  Depending on your distribution, run one of the following scripts as superuser:
        -   Debian (Ubuntu): `./tools/deps-ubuntu.sh`
        -   Red Hat (Fedora): `./tools/deps-fedora.sh`
        -   Arch (x86_64): `./tools/deps-arch.sh`

# Building

Open a terminal and go into the root folder of the project, which contains the top-level `CMakeLists.txt` file.
Do not run CMake directly from the top-level folder. Instead, issue the following commands from the terminal:

1.  `mkdir build`
2.  `cd build`
3.  Generate the project: `cmake ..`
4.  Build the project:
    -   Windows, one of the following:
        -   `cmake --build .`
        -   or just double-click the `.sln` file to open it in Visual Studio, then press `Ctrl+Shift+B` to build it
    -   Linux, one of the following:
        -   `cmake --build .`
        -   or just `make`

**Note:** If something goes wrong when generating the project, just delete the contents of the `build` folder, or the folder itself, then try again.

# Running

You can run the project from an IDE, as well as standalone, from anywhere on disk. For example:

-   Windows, one of the following:
    -   `.\bin\Debug\EGCFramework`
    -   or just open the `.sln` file in Visual Studio, then press `F5` to run it

-   Linux:
    -   `./bin/Debug/EGCFramework`


# Contributing

See the [CONTRIBUTING.md](CONTRIBUTING.md) file for more info.
A future roadmap is ***TBD***.


# License

This project is available under the [MIT][ref-mit] license. See [LICENSE.md](LICENSE.md) for the full license text.
This project also includes external libraries that are available under a variety of licenses. 


[ref-cmake]:            https://github.com/Kitware/CMake/
[ref-cmake-dl]:         https://cmake.org/download/
[ref-cmake-build]:      https://github.com/Kitware/CMake#building-cmake-from-scratch
[ref-igp-wiki]:         https://en.wikipedia.org/wiki/Graphics_processing_unit#Integrated_graphics_processing_unit
[ref-intelhd-wiki]:     https://en.wikipedia.org/wiki/Intel_Graphics_Technology
[ref-mit]:              https://opensource.org/licenses/MIT
