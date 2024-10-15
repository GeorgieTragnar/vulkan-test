#!/bin/bash

# Function to show usage
usage() {
    echo "Usage: $0 [-c] [-r]"
    echo "  -c    Perform a clean build (removes the build directory before building)"
    echo "  -r    Build in Release mode"
    exit 1
}

# Parse the command-line options
CLEAN_BUILD=0
BUILD_TYPE="Debug"

while getopts "cr" opt; do
    case ${opt} in
        c )
            CLEAN_BUILD=1
            ;;
        r )
            BUILD_TYPE="Release"
            ;;
        * )
            usage
            ;;
    esac
done

# Perform a clean build if -c is provided
if [ $CLEAN_BUILD -eq 1 ]; then
    echo "Performing clean build..."
    rm -rf build_vulkan-test/*
fi

# Create a build directory if it doesn't exist
mkdir -p build_vulkan-test
cd build_vulkan-test

# Compile shaders
~/repositories/vulkan-sdk/x86_64/bin/glslc ../vulkan-test/shaders/shader.vert -o vert.spv
~/repositories/vulkan-sdk/x86_64/bin/glslc ../vulkan-test/shaders/shader.frag -o frag.spv

if [ $CLEAN_BUILD -eq 1 ]; then
    # Run cmake with the specified GCC and G++ compilers and build type
    cmake ../vulkan-test/ -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_C_COMPILER=gcc-13 -DCMAKE_CXX_COMPILER=g++-13
fi

# Compile the project
make -j28

# Go back to the original directory (optional)
cd ..
