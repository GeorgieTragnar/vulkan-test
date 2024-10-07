
#include "Window.h"

#ifdef LOG_GROUP
	#undef LOG_GROUP 
#endif
#define LOG_GROUP LOG_GROUP_GLFW

Window::Window()
	: _glfwWindow(nullptr),
	_glfwWidth(0),
	_glfwHeight(0),
	_frameBufferResizeCallback(nullptr)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	_glfwWidth = 800;
	_glfwHeight = 600;
	_glfwWindow = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
	// Store 'this' in the window's user pointer
	glfwSetWindowUserPointer(_glfwWindow, this);

	// Set the framebuffer size callback using a non-capturing lambda
	glfwSetFramebufferSizeCallback(_glfwWindow, [](GLFWwindow* window, int width, int height) {
		// Retrieve 'this' pointer from the window's user pointer
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (self && self->_frameBufferResizeCallback) {
			// Invoke the _callback function
			self->_frameBufferResizeCallback();
		}
	});
}

Window::~Window() 
{
	glfwDestroyWindow(_glfwWindow);
	glfwTerminate();
}

bool Window::loopStep() 
{
	if (glfwWindowShouldClose(_glfwWindow))
		return false;
	glfwPollEvents();
	return true;
}

void Window::setFrameBufferResizeCallback(std::function<void(void)> cb)
{
	_frameBufferResizeCallback = std::move(cb);
}

const char** Window::getRequiredInstanceExtensions(uint32_t& count)
{
	return glfwGetRequiredInstanceExtensions(&count);
}

VkResult Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
{
	return glfwCreateWindowSurface(instance, _glfwWindow, nullptr, surface);
}

GLFWwindow* Window::get_glfwWindow()
{
	return _glfwWindow;
}
