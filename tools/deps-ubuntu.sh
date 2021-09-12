#!/bin/bash


# NOTE: You must run this script as superuser.


# NOTE: This script is designed for Ubuntu 18.04 or newer.
# We strongly advise against using anything older than that.


# Update package lists
sudo apt update

# Install basic development tools
sudo apt install -y build-essential

# Install required dependencies
sudo apt install -y cmake g++ libglm-dev libglew-dev libassimp-dev libglfw3-dev libspdlog-dev libfreetype-dev pkg-config
