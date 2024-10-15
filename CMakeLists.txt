cmake_minimum_required(VERSION 3.10)


# Setup vcpkg script with CMake (note: should be placed before project() call)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")


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

find_package(Vulkan REQUIRED)
find_package(FFMPEG REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(LibDataChannel CONFIG REQUIRED)


target_include_directories(VulkanProject PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_directories(VulkanProject PRIVATE ${FFMPEG_LIBRARY_DIRS})

target_link_libraries(VulkanProject PRIVATE
	Vulkan::Vulkan
	glfw
	PRIVATE ${FFMPEG_LIBRARIES}
	PRIVATE LibDataChannel::LibDataChannelStatic
)
