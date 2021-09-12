-   **[Docs home](../home.md)**

# Running on WSL2

## :information_source: Why would I want this?

If you have Windows and want to use (or develop) this project on Linux, but don't feel like using a virtual machine or installing Linux directly on your device, then you can use WSL2.

This is arguably the most powerful method for project development, as it allows you to run Windows and Linux executables side by side. To avoid breaking your computer, please only attempt this method if you know what you're doing.


## :white_check_mark: Prerequisites

### General stuff

-   Update your Windows 10. It needs to be version 1903 or higher, with Build 18362 or higher
    -   Run `winver.exe` from a terminal to find out your version
-   Chocolatey: https://chocolatey.org/install


### WSL2

Follow these steps:

1.  Follow steps 1 through 5 in [this Microsoft guide][ref-ms-wsl]. ***Carefully read everything*** before you proceed to follow the steps therein.
2.  At step 6 in the Microsoft guide, choose and install your preferred Linux distribution:
    -   Debian (Ubuntu): just install the latest Ubuntu you can find in the Microsoft Store
    -   Red Hat (Fedora): ***not recommended***. See `Installing other Linux distros` in this guide
    -   Arch (x86_64): ***not recommended***. See `Installing other Linux distros` in this guide
3.  Check that you actually have WSL2, by following [this guide][ref-askubuntu-wsl].


### Windows software

Follow these steps:

1.  Open the Start menu
2.  Type `cmd` but do ***not*** hit Enter
3.  Right-click on `cmd` and choose `Run as administrator`, and a terminal should appear
4.  Install `vcxsrv`:
    ```bat
    choco install -y vcxsrv
    ```
5.  Add an exception to the firewall: 
    ```bat
    netsh advfirewall firewall add rule name="vcxsrv" dir=in action=allow enable=yes profile=any program="C:\Program Files\VcXsrv\vcxsrv.exe"
    ```


### Linux software

Follow these steps:

1.  Open the Start menu
2.  Type `wsl` and hit Enter, and a Linux terminal should appear
3.  Update your package list
    ```sh
    sudo apt update                         # Debian (Ubuntu)
    sudo dnf check-update                   # Red Hat (Fedora)
    sudo pacman -Syu                        # Arch (x86_64)
    ```
5.  Install `xinit`:
    ```sh
    sudo apt install -y xinit              # Debian (Ubuntu)
    sudo dnf -y install xinit              # Red Hat (Fedora)
    sudo pacman -S xorg-xinit             # Arch (x86_64)
    ```


## :rocket: Running

At this point, you may ask what you just installed. `vcxsrv` (on Windows) and `xinit` (on Linux) are two graphical servers that work together, so that you can run graphical apps from a command-line connection. Now we need to link one another.

Follow these steps:

1.  From a Windows terminal, run:
    ```bat
    "C:\Program Files\VcXsrv\vcxsrv.exe" :0 -ac -terminate -lesspointer -multiwindow -clipboard -nowgl
    ```
2.  From a Linux terminal:
    1.  Run this command:
        ```sh
        export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2; exit;}'):0.0
        ```
    2.  Optionally, run `glxinfo` to check your OpenGL version:
        ```sh
        glxinfo | grep "version"
        ```
    3.  Optionally, run a random graphical application to check that everything works. For example:
        ```sh
        sudo apt install -y mesa-utils      # Debian (Ubuntu)
        sudo dnf -y install glx-utils       # Red Hat (Fedora)
        sudo pacman -S mesa-demos          # Arch (x86_64)

        glxgears
        ```
3.  You can now run ***any*** graphical application from inside Linux, including this project:
    ```sh
    ./bin/Debug/GFXFramework
    ```

That's it! :tada:


## :hammer_and_wrench: Installing other Linux distros

***This section is optional. Follow this section at your own risk.***

At the time of writing this guide, Fedora and Arch don't have official packages in the Microsoft Store, yet. We expect that this will change in the future.

If you want to go deeper, you can install these via WSL2 as well. For this to work, keep in mind that you need to already have an existing WSL2 Linux distribution, such as, you guessed it, Ubuntu. What follows is a series of instructions that may or may not work for you.


### WSL2 Fedora

Use the following instructions as a starting point:

1.  Go here: https://github.com/fedora-cloud/docker-brew-fedora/tree/33/x86_64
2.  Download this file: `fedora-33.20210106-x86_64.tar.xz`
3.  Prepare the image, from an already existing Linux terminal:
    ```sh
    unxz fedora-33.20210106-x86_64.tar.xz
    mv fedora-33.20210106-x86_64.tar fedora-33.tar
    ```
4.  Install and start the image, from a Windows terminal:
    ```bat
    mkdir c:\distros
    wsl --import fedora-33 c:\distros\fedora-33 fedora-33.tar
    wsl -d fedora-33
    ```
5.  Install basic stuff:
    ```sh
    sudo dnf check-update
    sudo dnf -y install wget curl sudo ncurses dnf-plugins-core dnf-utils passwd findutils
    ```
6.  Install any missing graphics drivers:
    ```sh
    sudo dnf -y install mesa-dri-drivers
    ```


### WSL2 Arch

Use the following instructions as a starting point:

1.  Go here: http://archlinux.uk.mirror.allworldit.com/archlinux/iso/2021.01.01/
2.  Download this file: `archlinux-bootstrap-2021.01.01-x86_64.tar.gz`
3.  Prepare the image, from an already existing Linux terminal:
    ```bat
    sudo tar xzf archlinux-bootstrap-2020.12.01-x86_64.tar.gz
    sudo tar czf arch-2020.12.01.tar.gz -C root.x86_64/ $(cd root.x86_64/ && ls)
    sudo rm -rf root.x86_64/
    sudo rm archlinux-bootstrap-2020.12.01-x86_64.tar.gz
    ```
4.  Install and start the image, from a Windows terminal:
    ```bat
    mkdir c:\distros
    wsl --import arch-2020.12.01 "c:\distros\arch-2020.12.01" arch-2020.12.01.tar.gz
    wsl -d arch-2020.12.01
    ```
5.  Set up the package sources:
    ```sh
    cd ~
    curl -s "https://git.archlinux.org/pacman-contrib.git/plain/src/rankmirrors.sh.in" > rankmirrors.sh
    chmod +x rankmirrors.sh
    curl -s "https://archlinux.org/mirrorlist/?country=US&protocol=http&protocol=https&ip_version=4" | cut -b 2- | ./rankmirrors.sh -n 10 - > /etc/pacman.d/mirrorlist
    ```
6.  Install basic stuff:
    ```sh
    pacman -Syu base-devel
    echo "en_US.UTF-8 UTF-8" > /etc/locale.gen
    locale-gen
    ```


[ref-ms-wsl]:           https://docs.microsoft.com/en-us/windows/wsl/install-win10#manual-installation-steps
[ref-ms-wsl-req]:       https://docs.microsoft.com/en-us/windows/wsl/install-win10#step-2---check-requirements-for-running-wsl-2
[ref-askubuntu-wsl]:    https://askubuntu.com/a/1177730
