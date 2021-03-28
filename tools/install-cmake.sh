#!/bin/sh

# Variables
cmk_ver=3.20.0
cmk_name=cmake-$cmk_ver-linux-x86_64
cmk_install_dir=~/opt


# Define cleanup handler
cleanup ()
{
    echo -n "Cleaning up... "

    [ -f $cmk_install_dir/$cmk_name.sh ] &&
        { rm -f $cmk_install_dir/$cmk_name.sh ; }

    echo "done"
}

# Define error handler
fail ()
{
    ret_code=$?

    echo -n "failed: "
    echo $1 >&2
    cleanup $ret_code

    exit $ret_code
}

# Define success handler
pass ()
{
    ret_code=$?

    cleanup $ret_code
    echo "Completed"

    exit $ret_code
}


# Create install dir
{
    echo -n "Creating installation dir... "

    mkdir -p $cmk_install_dir ||
        { fail "cannot create install dir" ; }

    cd $cmk_install_dir ||
        { fail "cannot access install dir" ; }

    echo "done"
}

# Download
{
    echo -n "Downloading, please wait... "

    [ -f $cmk_name.sh ] &&
        { fail "cmake installer already exists" ; }

    wget https://github.com/Kitware/CMake/releases/download/v$cmk_ver/$cmk_name.sh >/dev/null 2>&1 ||
        { fail "cannot download" ; }

    echo "done"
}

# Install
{
    echo -n "Installing... "

    [ -d $cmk_name ] &&
        { fail "CMake subdir already exists" ; }

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
pass
