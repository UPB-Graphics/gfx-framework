#!/bin/bash


# NOTE: You must run this script as superuser.


# Update package lists
sudo dnf check-update

# Install basic development tools
sudo dnf install -y @development-tools

# Install graphics drivers and utilities, including glxinfo and glxgears
sudo dnf install -y mesa-dri-drivers glx-utils

# Install required dependencies
sudo dnf install -y cmake g++ assimp-devel glfw-devel glew-devel glm-devel spdlog-devel freetype-devel pkg-config
