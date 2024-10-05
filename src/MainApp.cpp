
#include "MainApp.h"

#include "LogMacros.h"
#ifdef LOG_GROUP
	#undef LOG_GROUP 
#endif
#define LOG_GROUP LOG_GROUP_DEFAULT

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
	LOG_INFO("MainApp cleanup");
}