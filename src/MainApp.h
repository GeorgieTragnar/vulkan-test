
#pragma once

#include "headers.h"

class MainApp {
public:
	void run();

private:
	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();

	GLFWwindow* _glfwWindow = nullptr;

	int32_t _glfwWidth = 0;
	int32_t _glfwHeight = 0;
	
};