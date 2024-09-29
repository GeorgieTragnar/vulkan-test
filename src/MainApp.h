
#pragma once

#include "headers.h"
#include "Window.h"
#include "VulkanApp.h"

class MainApp 
{
public:
	void run();

private:
	void mainLoop();
	void cleanup();

	
};