
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <assert.h>
#include <cstring>
#include <optional>
#include <set>
#include <sstream>

#define LOG_GROUP_DEFAULT "LogGroupDefault"
#define LOG_GROUP_VULKAN "LogGroupVulkan"
#define LOG_GROUP_GLFW "LogGroupGLFW"
