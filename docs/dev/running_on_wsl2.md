-   **[Docs home](../home.md)**

# Running on WSL2

## :information_source: Why would I want this?

If you have Windows and want to use (or develop) this project on Linux, but don't feel like using a virtual machine or installing Linux directly on your device, then you can use WSL2.

This is arguably the most powerful method for project development, as it allows you to run Windows and Linux executables side by side.


## :white_check_mark: Prerequisites

This guide has been written for **Windows 11 22H2**. The installation process for WSL2 may differ for Windows 10, and even for older versions of Windows 11, and is not described here.

This guide also assumes that you have the most recent graphics drivers. In particular:

-   If you have an Intel CPU, the graphics driver must be version `31.0.101.xxxx` or later. Update the driver from:
        Settings
        -> Windows Update
        -> Advanced options
        -> Optional updates
        -> Intel Corporation - Display - `<version>`


## :hammer_and_wrench: Installing WSL2 + Ubuntu

This method allows you to install Ubuntu on Windows.

-   Open a terminal and run `wsl -l -v`
    -   If you get a table similar to this one, then you already have WSL2 + Ubuntu:
        ```
          NAME      STATE           VERSION
        * Ubuntu    Running         2
        ```
    -   Otherwise, follow the next steps
-   Run `wsl --install Ubuntu`
-   If prompted, restart your computer
-   Wait for the installation to complete
-   Choose your username and password
-   To access Ubuntu from Windows:
    -   Start menu -> type `Ubuntu` -> open the app
    -   or, open a terminal and run `wsl`


## :hammer_and_wrench: Installing other Linux distributions (the easy way)

This method allows you to install Linux distributions from the Microsoft Store.

-   Open a terminal and run `wsl -l -o`
-   Choose your preferred distribution and run `wsl --install <NAME>`, for example `wsl --install Debian`
-   To access it from Windows:
    -   Start menu -> type the name -> open the app
    -   or, open a terminal and run `wsl -d <NAME>`, for example `wsl -d Debian`


## :hammer_and_wrench: Installing other Linux distributions (the hard way)

You can install any Linux distribution on Windows. Some examples are given below. Note that you must already have WSL2 + Ubuntu installed for this to work.

Follow this section at your own risk, as the instructions herein are not thoroughly explained, and neither are all situations when something may go wrong. To avoid breaking your computer, please only attempt this method if you know what you're doing.


### WSL2 Fedora

Use the following instructions as a starting point:

1.  From a WSL2 Ubuntu terminal, download the OS image using the following commands:
    ```sh
    distro_url=https://github.com/fedora-cloud/docker-brew-fedora/
    latest_version=$(git ls-remote ${distro_url} 'refs/heads/*' | awk '{print $2}' | grep "[0-9]" | sort | tail -1 | tr -d -c 0-9)
    temp_dir=/mnt/c/LinuxDistroDownloads/Fedora_${latest_version}
    ar_name=fedora-${latest_version}-x86_64.tar.xz
    download_url=https://raw.githubusercontent.com/fedora-cloud/docker-brew-fedora/${latest_version}/x86_64/${ar_name}

    mkdir -p ${temp_dir}
    cd ${temp_dir}
    curl -O ${download_url}
    ```

2.  From a Windows terminal, install the OS image using following commands. Make sure to set `latest_version` to the proper value:
    ```bat
    set latest_version=40
    mkdir C:\LinuxDistroInstallations
    wsl --import Fedora_%latest_version% C:\LinuxDistroInstallations\Fedora_%latest_version% C:\LinuxDistroDownloads\Fedora_%latest_version%\fedora-%latest_version%-x86_64.tar.xz
    wsl -d Fedora_%latest_version%
    ```

3.  From the distribution's terminal, prepare the OS:
    ```sh
    # install basic packages
    sudo dnf check-update
    sudo dnf -y install wget curl sudo ncurses dnf-plugins-core dnf-utils passwd findutils
    
    # install the graphics drivers
    sudo dnf -y install mesa-dri-drivers
    ```


### WSL2 Arch

Use the following instructions as a starting point:

1.  From a WSL2 Ubuntu terminal, download the OS image using the following commands:
    ```sh
    distro_url=https://archlinux.uk.mirror.allworldit.com/archlinux/iso/latest/
    latest_version=$(curl -s ${distro_url} | grep -o '[0-9]*\.[0-9]*\.[0-9]*' | sort | tail -1)
    temp_dir=/mnt/c/LinuxDistroDownloads/Arch_${latest_version}
    ar_name=archlinux-bootstrap-${latest_version}-x86_64.tar.gz
    download_url=https://archlinux.uk.mirror.allworldit.com/archlinux/iso/latest/${ar_name}

    mkdir -p ${temp_dir}
    cd ${temp_dir}
    curl -O ${download_url}

    tar xzf ${ar_name}
    tar czf _${ar_name} -C root.x86_64/ $(cd root.x86_64/ && ls)
    rm -rf root.x86_64/
    rm ${ar_name}
    ```

2.  From a Windows terminal, install the OS image using following commands. Make sure to set `latest_version` to the proper value:
    ```bat
    set latest_version=2023.09.01
    mkdir C:\LinuxDistroInstallations
    wsl --import Arch_%latest_version% C:\LinuxDistroInstallations\Arch_%latest_version% C:\LinuxDistroDownloads\Arch_%latest_version%\_archlinux-bootstrap-%latest_version%-x86_64.tar.gz
    wsl -d Arch_%latest_version%
    ```

3.  From the distribution's terminal, prepare the OS:
    ```sh
    # set up the package sources
    cd ~
    curl -s "https://gitlab.archlinux.org/pacman/pacman-contrib/-/raw/master/src/rankmirrors.sh.in" > rankmirrors.sh
    chmod +x rankmirrors.sh
    curl -s "https://archlinux.org/mirrorlist/?country=US&protocol=http&protocol=https&ip_version=4" | cut -b 2- | ./rankmirrors.sh -n 10 - > /etc/pacman.d/mirrorlist

    # set up various things
    echo "en_US.UTF-8 UTF-8" > /etc/locale.gen
    locale-gen
    pacman-key --init
    pacman-key --populate archlinux
    sudo chmod 755 -R /usr

    # install basic packages
    pacman -Syu base-devel
    ```
    
