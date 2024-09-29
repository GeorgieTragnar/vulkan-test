
#include "MainApp.h"


void MainApp::run() 
{
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

void MainApp::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	_glfwWidth = 800;
	_glfwHeight = 600;
	_glfwWindow = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
}

void MainApp::initVulkan() 
{

}

void MainApp::mainLoop() 
{
	while (!glfwWindowShouldClose(_glfwWindow))
	{
		glfwPollEvents();
	}
}

void MainApp::cleanup() 
{
	glfwDestroyWindow(_glfwWindow);
	glfwTerminate();
}