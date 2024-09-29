
#pragma once

#include "headers.h"

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


};
