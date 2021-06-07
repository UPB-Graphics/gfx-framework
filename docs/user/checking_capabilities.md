-   **[Docs home](../home.md)**

# Checking your graphics capabilities

First things first, ***update your graphics drivers***. This guide assumes that your drivers are up-to-date. Instructions on how to install or update drivers differ across manufacturers and are ***not*** covered here.


## Finding supported OpenGL versions

If your computer has a processor manufactured within the last few years, you should be safe: a modern CPU has [graphics capabilities itself][ref-igp-wiki], see for example [Intel HD Graphics][ref-intelhd-wiki]. If you have a dedicated graphics card, you should be even safer.

Use the steps in this section to find out what OpenGL versions your device supports.


## Windows

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


## Linux

1.  Run one of the following commands:
    -   Debian (Ubuntu): `sudo apt install -y mesa-utils`
    -   Red Hat (Fedora): `sudo dnf install -y glx-utils`
    -   Arch (x86_64): `sudo pacman -S mesa-demos`
2.  Run the following command:
    ```bash
    glxinfo | grep "core profile version string"
    ```
3.  Check the output, for example:
    ```
    OpenGL core profile version string: 3.3 (Core Profile) Mesa 20.0.8
    ```

## macOS

-   Versions 10.9 (Mavericks) through 10.14 (Mojave) are expected to work out-of-the-box.
-   Versions 10.15 (Catalina) and 11.0 (Big Sur) are also expected to work, even though Apple has deprecated OpenGL.


[ref-igp-wiki]:         https://en.wikipedia.org/wiki/Graphics_processing_unit#Integrated_graphics_processing_unit
[ref-intelhd-wiki]:     https://en.wikipedia.org/wiki/Intel_Graphics_Technology
