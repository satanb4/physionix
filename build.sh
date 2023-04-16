#!/usr/bin/env bash

# This script builds the project
base_dir="$(pwd)"
build_dir="${base_dir}/build"
src_dir="${base_dir}/src"

sensor_dir="${src_dir}/EMGSensor"
processing_dir="${src_dir}/EMGProcessing"
api_dir="${src_dir}/EMGApi"
final_build="physionix"


# Create the build directory
echo 'Creating build directory...'
if [ -d $build_dir ]; then
    echo "Found existing build directory. Removing..." | tee -a $build_log
    rm -rf $build_dir
fi
mkdir -p $build_dir 

build_log="${build_dir}/build_log.txt"
if ! [ -f $build_log ]; then
    touch $build_log
fi

# Install Dependencies
echo 'Installing dependencies...'
dependency_script="${base_dir}/scripts/installdependencies.sh"
if [ -f $dependency_script ]; then
    echo "Found dependency script. Installing..." | tee -a $build_log
    chmod +x $dependency_script
    $dependency_script
else
    echo "Dependency script not found. Skipping..." | tee -a $build_log
fi

# Build the project
echo 'Building project...'
echo "Building EMGSensor..." | tee -a $build_log
cd $src_dir
make
mv *.o $final_build $build_dir
echo "Build complete." | tee -a $build_log

# Build the EMGApi
echo "Building EMGApi..." | tee -a $build_log
api_output="${api_dir}/PhysionixServer"
site_dir="${api_dir}/website"
if [ -d $api_dir ]; then
    cd $api_dir
    cmake CMakeLists.txt
    make
    mv *.o $api_output $build_dir
    rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake Makefile
    echo "Build complete." | tee -a $build_log
    echo "Copying website files..." | tee -a $build_log
    if [ -d $site_dir ]; then
        echo "Found website directory. Copying..." | tee -a $build_log
        cp -r $site_dir $build_dir
        echo "Website files copied." | tee -a $build_log
    else
        echo "Website directory not found. Skipping..." | tee -a $build_log
    fi
else
    echo "EMGApi not found. Skipping..." | tee -a $build_log
fi