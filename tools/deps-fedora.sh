#!/bin/bash


# NOTE: You must run this script as superuser.


# Install basic development tools
sudo dnf install -y @development-tools

# Install graphics drivers and utilities, including glxinfo and glxgears.
sudo dnf install -y mesa-dri-drivers glx-utils

# Install required tools for this project.
# Package list originally prepared by Catalin Puscoci.
sudo dnf install -y cmake g++ assimp-devel glfw-devel glew-devel glm-devel pkg-config
