#!/bin/bash


# NOTE: You must run this script as superuser.


# NOTE: This script is designed for Ubuntu 18.04 or newer.
# We strongly advise against using anything older than that.


# Install basic development tools
sudo apt install -y build-essential


# Install required tools for this project.
sudo apt install -y cmake g++ libglm-dev libglew-dev libassimp-dev libglfw3-dev
