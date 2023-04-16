#!/usr/bin/env bash

# This script installs dependencies for the project
build_dir="$(pwd)/build"
build_log="${build_dir}/build_log.txt"

# Install the i2c tools
echo 'Installing i2c tools...'
cmd="dpkg -s i2c-tools"
if !($cmd | grep "ok installed"); then
    echo "i2c tools not found. Installing..." | tee -a $build_log
    sudo apt-get update
    sudo apt-get install -y i2c-tools >> $build_log
fi

base_dir="$(pwd)"
packages="${base_dir}/packages"

fftw_zip="${packages}/fftw-3.3.10.tar.gz"
fftw_dir="${packages}/fftw-3.3.10"

# Path: scripts/installdependencies.sh
# Build the fftw library
if [ ! -f '/usr/local/bin/fftw-wisdom' ]; then
    echo 'Building fftw library...' | tee -a $build_log
    if [ -f $fftw_zip ]; then
        echo "Found fftw zip. Building..." | tee -a $build_log
        cd $packages
        tar -xzf $fftw_zip
        cd $fftw_dir
        ./configure
        make
        sudo make install
        cd ..
        sudo rm -rf $fftw_dir
        cd $base_dir
        echo "fftw build complete." | tee -a $build_log
    else
        echo "fftw zip not found. Skipping..." | tee -a $build_log
    fi
else
    echo "fftw already installed. Skipping..." | tee -a $build_log
fi

if [[ $(grep -i 'raspbian' /etc/os-release) ||  $ACTIONS = true ]]; then
    if [ "$ACTIONS" = true ]; then
        echo "Running in GitHub Actions. Installing pigpio..."  | tee -a $build_log
    else
        echo "Raspbian OS detected. Installing pigpio..." | tee -a $build_log
    fi
    sudo apt-get install -y pigpio libfcgi-dev libjsoncpp-dev >> $build_log
else
    echo "Non-Raspbian OS detected. Skipping Raspbian specific dependencies..."
    sudo apt-get install -y pigpio-tools libfcgi-dev libjsoncpp-dev >> $build_log
fi

# Build the jsonfastcgi interface library
echo 'Building jsonfastcgi library...' | tee -a $build_log
api_dir="${base_dir}/src/EMGApi"
cgi_dir="${api_dir}/fastcgi"
if [ -d $cgi_dir ]; then
    echo "Found jsonfastcgi directory. Building..." | tee -a $build_log
    cd $cgi_dir
    cmake CMakeLists.txt
    sudo make install json_fastcgi_web_api.h | tee -a $build_log
    rm -rf CMakeCache.txt CMakeFiles cmake_install.cmake
    cd $base_dir
    echo "jsonfastcgi build complete." | tee -a $build_log
else
    echo "jsonfastcgi directory not found. Skipping..." | tee -a $build_log
fi