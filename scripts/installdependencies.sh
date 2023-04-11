#!/usr/bin/env bash

# Install the i2c tools
echo 'Installing i2c tools...'
cmd="dpkg -s i2c-tools"
if !($cmd | grep "ok installed"); then
    echo "i2c tools not found. Installing..."
    sudo apt-get update
    sudo apt-get install -y i2c-tools
fi

base_dir="$(pwd)"
packages="${base_dir}/packages"

fftw_zip="${packages}/fftw-3.3.10.tar.gz"
fftw_dir="${packages}/fftw-3.3.10"

# Path: scripts/installdependencies.sh
# Build the fftw library
if [ ! -f '/usr/local/bin/fftw-wisdom' ]; then
    echo 'Building fftw library...'
    if [ -f $fftw_zip ]; then
        echo "Found fftw zip. Building..."
        cd $packages
        tar -xzf $fftw_zip
        cd $fftw_dir
        ./configure
        make
        sudo make install
        cd ..
        sudo rm -rf $fftw_dir
        cd $base_dir
        echo "fftw build complete."
    else
        echo "fftw zip not found. Skipping..."
    fi
else
    echo "fftw already installed. Skipping..."
fi
