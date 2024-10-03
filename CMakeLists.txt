cmake_minimum_required(VERSION 3.10)
project(VulkanProject)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(VulkanProject ${SRC_FILES})

# Find Vulkan and link it
find_package(Vulkan REQUIRED)
target_link_libraries(VulkanProject Vulkan::Vulkan)

# Find GLFW and link it
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(VulkanProject glfw)
