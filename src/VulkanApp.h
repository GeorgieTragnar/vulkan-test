
#pragma once

#include "headers.h"
#include "Window.h"

class VulkanApp
{
public:
	static VulkanApp& Instance()
	{
		static VulkanApp inst;
		return inst;
	}



protected:

	VulkanApp();
	~VulkanApp();

	void createInstance();
	void checkExtensions();



	VkInstance _instance = nullptr;

};
