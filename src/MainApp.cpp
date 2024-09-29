
#include "MainApp.h"


void MainApp::run() 
{
	Window::Instance();
	VulkanApp::Instance();
	mainLoop();
	cleanup();
}

void MainApp::mainLoop() 
{
	while (Window::Instance().loopStep())
	{

	}
}

void MainApp::cleanup() 
{
	
}