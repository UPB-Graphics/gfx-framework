-   **[Docs home](../docs.md)**

# Checking your graphics capabilities


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
        1.  Run one of the following scripts:
            -   Debian (Ubuntu): `sudo apt install -y mesa-utils`
            -   Red Hat (Fedora): `sudo dnf install -y glx-utils`
            -   Arch (x86_64): `sudo pacman -Sy mesa-demos`
        2.  Run the following command:
            ```bash
            glxinfo | grep "core profile version string"
            ```
        4.  Check the output, for example:
            ```
            OpenGL core profile version string: 3.3 (Core Profile) Mesa 20.0.8
            ```

    -   Linux, if installed via WSL2: if you have Windows and want to use (or develop) this project on Linux, but don't feel like using a virtual machine or installing Linux directly on your device, then you can use WSL2. This option is slightly more adventurous, and you should only try it if you know what you're doing. For more details, check the documentation.

    -   macOS:
        -   Versions 10.9 (Mavericks) through 10.14 (Mojave) should work but are untested.
        -   Versions 10.15 (Catalina) and 11.0 (Big Sur) should also work, even though Apple has deprecated OpenGL.
