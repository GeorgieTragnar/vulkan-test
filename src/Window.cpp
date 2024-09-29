
#include "Window.h"

#ifdef LOG_GROUP
	#undef LOG_GROUP 
#endif
#define LOG_GROUP LOG_GROUP_GLFW

Window::Window()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	_glfwWidth = 800;
	_glfwHeight = 600;
	_glfwWindow = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
}

Window::~Window() 
{
	glfwDestroyWindow(_glfwWindow);
	glfwTerminate();
}


bool Window::loopStep() 
{
	if (!glfwWindowShouldClose(_glfwWindow))
		return false;
	glfwPollEvents();
	return true;
}

const char** Window::getRequiredInstanceExtensions(uint32_t& count)
{
	return glfwGetRequiredInstanceExtensions(&count);
}
