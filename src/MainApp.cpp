
#include "MainApp.h"

#include "LogMacros.h"
#ifdef LOG_GROUP
	#undef LOG_GROUP 
#endif
#define LOG_GROUP LOG_GROUP_DEFAULT

MainApp::MainApp()
{
	run();
}

MainApp::~MainApp()
{
	LOG_INFO("MainApp cleanup");
}

void MainApp::run() 
{
	Window::Instance();
	VulkanApp::Instance();
	mainLoop();
}

void MainApp::mainLoop() 
{
	while (Window::Instance().loopStep())
	{
		drawFrame();
	}
}

void MainApp::drawFrame()
{

}
