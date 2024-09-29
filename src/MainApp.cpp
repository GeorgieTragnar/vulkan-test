
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
	LOG_TRACE("KHGSH");
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