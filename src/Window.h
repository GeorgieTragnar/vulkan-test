
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
	VkResult createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
	void setFrameBufferResizeCallback(std::function<void(void)> cb);

	GLFWwindow* get_glfwWindow();

protected:

	Window();
	~Window();

	GLFWwindow* _glfwWindow = nullptr;

	std::function<void(void)> _frameBufferResizeCallback;

	int32_t _glfwWidth;
	int32_t _glfwHeight;

};
