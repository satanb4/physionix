#!/usr/bin/env bash

# This script builds the project
base_dir="$(pwd)"
build_dir="${base_dir}/build"
src_dir="${base_dir}/src"

sensor_dir="${src_dir}/EMGSensor"
output_name1="emgSensor"

processing_dir="${src_dir}/EMGProcessing"
output_name2="emgFilter"

# Create the build directory
echo 'Creating build directory...'
if [ -d $build_dir ]; then
    echo "Found existing build directory. Removing..."
    rm -rf $build_dir
fi
mkdir -p $build_dir


# Install Dependencies
echo 'Installing dependencies...'
dependency_script="${base_dir}/scripts/installdependencies.sh"
if [ -f $dependency_script ]; then
    echo "Found dependency script. Installing..."
    chmod +x $dependency_script
    $dependency_script
else
    echo "Dependency script not found. Skipping..."
fi

# Build the project
echo 'Building project...'
echo "Building EMGSensor..."
cd $sensor_dir/sample-application
make
mv *.o $output_name1 $build_dir
cd $processing_dir/sample-application
make
mv *.o $output_name2 $build_dir
echo "Build complete."
