#!/bin/sh

if [[ $(command -v brew) == "" ]]; then
    # Package manager is not installed
    echo "Hombrew not installed, get it from https://brew.sh/"
else
    # Update package lists
    brew update

    # Install required dependencies
    brew install glew glfw assimp spdlog freetype pkgconfig
fi
