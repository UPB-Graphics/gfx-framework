#!/usr/bin/bash

# Run this script as root, obviously

# Should already be installed
# pacman -Syu base-devel

# It's assumed a functional graphics driver is installed and in-use, also X11
# If glxgears works, you should be fine
# https://wiki.archlinux.org/index.php/xorg
pacman -Syu cmake assimp glm glew glfw-x11 mesa lib32-mesa

# For proprietary nvidia drivers
# pacman -Syu nvidia-utils lib32-nvidia-utils
