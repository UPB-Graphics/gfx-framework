#!/usr/bin/bash


# NOTE: You must run this script as superuser.


# NOTE: If you installed Arch directly on your device, or in a
# virtual machine, but not via WSL2, you need to check that you
# have a working X11 display server (meaning, your Arch is not
# purely command-line).


# NOTE: If you have an Nvidia card, you can install the package `nvidia-utils`
# that contains Nvidia's proprietary drivers. For 32-bit application support,
# you can install `lib32-nvidia-utils`.


# Update package lists
sudo pacman -Syu

# Install basic development tools
# Contents: https://archlinux.org/groups/x86_64/base-devel/
sudo pacman -S base-devel

# Install graphics drivers and utilities, including glxinfo and glxgears.
# For 32-bit application support, also install the `lib32-*` packages.
sudo pacman -S mesa mesa-demos

# Install required dependencies
sudo pacman -S cmake assimp glm glew glfw-x11 spdlog freetype2 pkgconf
