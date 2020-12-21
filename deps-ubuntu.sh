#!/bin/bash
# script pus cap la cap de Valentin Radu
# folosind si munca pretioasa de Tiberiu Lepadatu
#
# pentru Ubuntu 16.04 (versiuni mai vechi, in principiu, este necesara o
# versiune mai noua de gcc si g++, care se poate obtine astfel:
# sudo add-apt-repository ppa:ubuntu-toolchain-r/test
# sudo apt update
# sudo apt install gcc-8 g++-8
# sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 80 --slave /usr/bin/g++ g++ /usr/bin/g++-8
#
# activeaza repository-ul universe, in care se gasesc anumite pachete necesare
sudo add-apt-repository "deb http://archive.ubuntu.com/ubuntu $(lsb_release -sc) universe"
sudo apt update
# instaleaza dependintele
sudo apt install cmake g++ libglm-dev libglew-dev libassimp-dev libglfw3-dev