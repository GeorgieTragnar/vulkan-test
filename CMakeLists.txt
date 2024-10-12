cmake_minimum_required(VERSION 3.10)
project(VulkanProject)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_custom_command(
	OUTPUT ${CMAKE_BINARY_DIR}/textures
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/textures ${CMAKE_BINARY_DIR}/textures
	DEPENDS ${CMAKE_SOURCE_DIR}/textures
	COMMENT "Creating symlink to textures directory"
)

add_custom_command(
	OUTPUT ${CMAKE_BINARY_DIR}/models
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/models ${CMAKE_BINARY_DIR}/models
	DEPENDS ${CMAKE_SOURCE_DIR}/models
	COMMENT "Creating symlink to models directory"
)

add_custom_target(CreateSymLinks ALL
    DEPENDS ${CMAKE_BINARY_DIR}/textures ${CMAKE_BINARY_DIR}/models
)

# Add the executable
file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(VulkanProject ${SRC_FILES})

# Find Vulkan and link it
find_package(Vulkan REQUIRED)
target_link_libraries(VulkanProject Vulkan::Vulkan)

# Find GLFW and link it
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(VulkanProject glfw)
