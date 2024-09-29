
#pragma once

#include "headers.h"

class Window
{
public:
	static Window& Instance()
	{
		static Window inst;
		return inst; 
	}

	bool loopStep();

	const char** getRequiredInstanceExtensions(uint32_t& count);

protected:

	Window();
	~Window();

	GLFWwindow* _glfwWindow = nullptr;

	int32_t _glfwWidth = 0;
	int32_t _glfwHeight = 0;

};
