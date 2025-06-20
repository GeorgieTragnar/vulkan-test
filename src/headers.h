
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <tiny_obj_loader.h>

#include <chrono>
#include <stdexcept>
#include <vector>
#include <assert.h>
#include <cstring>
#include <optional>
#include <set>
#include <sstream>
#include <algorithm>
#include <limits>
#include <fstream>
#include <functional>
#include <atomic>
#include <array>

#define LOG_GROUP_DEFAULT "LogGroupDefault"
#define LOG_GROUP_VULKAN "LogGroupVulkan"
#define LOG_GROUP_GLFW "LogGroupGLFW"
#define LOG_GROUP_FFMPEG "LogGroupFFMPEG"


static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

	size_t fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}
