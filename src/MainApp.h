
#pragma once

#include "headers.h"
#include "Window.h"
#include "VulkanApp.h"
#include "VideoStream.h"
#include "libdatachannel_streamer/Streamer.h"

class MainApp 
{
public:
	static MainApp& Instance()
	{
		static MainApp inst;
		return inst;
	}

protected:
	MainApp();
	~MainApp();


	void run();
	void mainLoop();

	
};