#!/usr/bin/env bash

# This script builds the project
base_dir="$(pwd)"
build_dir="${base_dir}/build"
src_dir="${base_dir}/src"

sensor_dir="${src_dir}/EMGSensor"
processing_dir="${src_dir}/EMGProcessing"
final_build="physionix"


build_log="${build_dir}/build_log.txt"
if ! [ -f $build_log ]; then
    touch $build_log
fi

# Create the build directory
echo 'Creating build directory...'
if [ -d $build_dir ]; then
    echo "Found existing build directory. Removing..." | tee -a $build_log
    rm -rf $build_dir
fi
mkdir -p $build_dir 


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
