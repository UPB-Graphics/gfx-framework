# EGX Framework

## :sparkles: Introduction

This project is a tiny graphics framework used by the Graphics Department of the Polytechnic University of Bucharest.
It is currently used as teaching and study material for a number of courses of increasing complexity:

-   **`egc`** Elements of Computer Graphics, BSc year 3 &mdash; [course materials (RO)](https://ocw.cs.pub.ro/courses/egc)
-   **`spg`** Graphics Processing Systems, BSc year 4 &mdash; [course materials (RO)](https://ocw.cs.pub.ro/courses/spg)

You can read more about it [in the docs](docs/home.md).

It has missing and closed-source functionality that you will need to implement.

The code is cross-platform, and you can build it natively on:

-   Windows: `i686`, `x86_64`, `aarch64`
-   Linux: `i686`, `x86_64`, `aarch64`
-   macOS: `x86_64`

Basically, it works on your computer.


## :white_check_mark: Prerequisites

This section describes ***what you need to do and install*** before actually building the code.


### Install a compiler

-   Minimum:
    -   Windows: Visual Studio 2015 Update 3 with `Programming Languages -> Visual C++` checked when installing
    -   Linux: `gcc g++` version 5
    -   macOS: `clang clang++` version 4

-   ***Strongly*** recommended:
    -   Windows: Visual Studio 2019 with `Workloads -> Desktop development with C++` checked when installing
    -   Linux: `g++` latest
    -   macOS: `clang++` latest
        -   either use `brew install llvm` or install XCode


### Check your graphics capabilities

Graphics capabilities are decided by the combination of your computer's hardware, drivers, and operating system.

This project requires OpenGL version ***3.3 core profile*** or newer for the simpler parts, and version ***4.1 core profile*** or newer for the more advanced parts. If you have a computer manufactured within the last few years, you should be safe. ***If you're not sure,*** follow the steps in [this guide](docs/user/checking_capabilities.md) to find out.


### Install the build tools

This project uses [CMake][ref-cmake]. It a nutshell, CMake doesn't compile your code, instead it creates files that you then use to compile your code (for example, it creates a Makefile on Linux and macOS, a Visual Studio project on Windows, and so on).

This project requires version ***3.16*** or newer, however, as with the compilers, it is ***strongly*** recommended to use the latest version. To install it, follow these steps:

-   Windows:
    1.  Go to the [CMake downloads page][ref-cmake-dl]
    2.  Download the latest version of the file called `cmake-<VERSION>-win64-x64.msi`
    3.  Install it

-   Linux:
    1.  Use your package manager to install `cmake`
    2.  Check the version using `cmake --version`
    3.  Depending on the version:
        -   if it's the minimum required (see above), you're all set
        -   otherwise, run `./tools/install-cmake.sh && . ~/.profile` in a terminal

-   macOS:
    1.  `brew install cmake`

After installation, run `cmake --version` (again) to check that it's in your `PATH` environment variable. This should happen automatically, but if it didn't, just add it manually. Instructions on how to add an executable to your `PATH` differ across operating systems and are ***not*** covered here.


### Install the third-party libraries

There are some open-source libraries that this project uses. To install them:

-   Windows: you don't need to do anything - all necessary libraries are already provided with the code

-   Linux:
    1.  Depending on your distribution, run one of the following scripts as superuser:
        -   Debian (Ubuntu): `./tools/deps-ubuntu.sh`
        -   Red Hat (Fedora): `./tools/deps-fedora.sh`
        -   Arch (x86_64): `./tools/deps-arch.sh`

-   macOS: `./tools/deps-macos.sh`


## :gear: Building

Open a terminal and go into the root folder of the project, which contains the top-level `CMakeLists.txt` file.
Do not run CMake directly from the top-level folder (meaning, do not do this: `cmake .`). Instead, make a separate directory, as follows:

1.  `mkdir build`
2.  `cd build`
3.  Generate the project:
    -   for EGC labs (default): `cmake ..`
    -   for SPG labs: `cmake .. -DWITH_LAB_EGC=0 -DWITH_LAB_SPG=1`
    -   for extra labs: `cmake .. -DWITH_LAB_EGC=0 -DWITH_LAB_EXTRA=1`
    -   for none (`SimpleScene` only): `cmake .. -DWITH_LAB_EGC=0`
4.  Build the project:
    -   Windows, one of the following:
        -   `cmake --build .`
        -   or just double-click the `.sln` file to open it in Visual Studio, then press `Ctrl+Shift+B` to build it
    -   Linux and macOS, one of the following:
        -   `cmake --build .`
        -   or just `make`

That's it! :tada:


### Rebuilding

It's very simple to rebuild:

-   Every time you modify source code and want to recompile, you only need to follow ***the last step*** (for example, just `make` again)
-   Every time you add/remove/rename a source code file on disk, you need to follow ***the last two steps*** (for example, just `cmake .. && make` again)
-   If something goes wrong when generating the project, just delete the contents of the `build` folder, or the folder itself, then follow all the steps again


## :rocket: Running

You can run the project from an IDE, as well as standalone, from anywhere on disk. For example:

-   Windows, one of the following:
    -   `.\bin\Debug\EGXFramework`
    -   or just open the `.sln` file in Visual Studio, then press `F5` to run it

-   Linux and macOS:
    -   `./bin/Debug/EGXFramework`

To run a certain lab:

-   Go into `main.cpp`
-   Find this line:
    ```cpp
    World *world = new egxc::SimpleScene();
    ```
-   Replace it with whatever you want to run, for example:
    ```cpp
    World *world = new egc::Laborator1();
    World *world = new spg::Laborator1();
    World *world = new extra::TessellationShader();
    // etc.
    ```


## :book: Documentation

All user and developer documentation can be found in the `docs` directory.


## :wrench: Contributing

See the [CONTRIBUTING.md](CONTRIBUTING.md) file for more info.
A future roadmap is ***TBD***.


## :page_facing_up: License

This project is available under the [MIT][ref-mit] license. See [LICENSE.md](LICENSE.md) for the full license text.
This project also includes external libraries that are available under a variety of licenses. 


[ref-cmake]:            https://github.com/Kitware/CMake/
[ref-cmake-dl]:         https://github.com/Kitware/CMake/releases/
[ref-cmake-build]:      https://github.com/Kitware/CMake#building-cmake-from-scratch
[ref-mit]:              https://opensource.org/licenses/MIT
