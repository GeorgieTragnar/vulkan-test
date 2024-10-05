#!/bin/bash

# Function to show usage
usage() {
    echo "Usage: $0 [-c]"
    echo "  -c    Perform a clean build (removes the build directory before building)"
    exit 1
}

# Parse the command-line options
CLEAN_BUILD=0

while getopts "c" opt; do
    case ${opt} in
        c )
            CLEAN_BUILD=1
            ;;
        * )
            usage
            ;;
    esac
done

# Perform a clean build if -c is provided
if [ $CLEAN_BUILD -eq 1 ]; then
    echo "Performing clean build..."
    rm -rf build_vulkan-test
fi

# Create a build directory if it doesn't exist
mkdir -p build_vulkan-test
cd build_vulkan-test

# Compile shaders
~/repositories/vulkan-sdk/x86_64/bin/glslc ../vulkan-test/shaders/shader.vert -o vert.spv
~/repositories/vulkan-sdk/x86_64/bin/glslc ../vulkan-test/shaders/shader.frag -o frag.spv

# Run cmake with the specified GCC and G++ compilers
cmake ../vulkan-test/ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc-13 -DCMAKE_CXX_COMPILER=g++-13

# Compile the project
make -j28

# Go back to the original directory (optional)
cd ..
