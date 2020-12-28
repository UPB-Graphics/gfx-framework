#!/bin/sh

# Variables
cmk_ver=3.17.5
cmk_name=cmake-$cmk_ver-Linux-x86_64
cmk_install_dir=~/opt

# Define error handler
fail () {
    ret_code=$?
    echo $1 >&2
    exit $ret_code
}

# Create install dir
{
    echo -n "Creating install dir... "

    mkdir -p $cmk_install_dir ||
        { fail "cannot create install dir" ; }

    cd $cmk_install_dir ||
        { fail "cannot access install dir" ; }

    echo "done"
}

# Download
{
    echo -n "Downloading, please wait... "

    wget https://github.com/Kitware/CMake/releases/download/v$cmk_ver/$cmk_name.sh >/dev/null 2>&1 ||
        { fail "cannot download" ; }

    echo "done"
}

# Install
{
    echo -n "Installing... "

    chmod +x $cmk_name.sh ||
        { fail "cannot change installer permissions" ; }

    yes | ./$cmk_name.sh >/dev/null ||
        { fail "cannot install" ; }

    echo "done"
}

# Add to system path
{
    echo -n "Adding to system path... "

    echo "\nPATH=$cmk_install_dir/$cmk_name/bin:\$PATH" >> ~/.profile ||
        { fail "cannot write profile file" ; }

    echo "done"
}

# Exit
echo "Completed"
