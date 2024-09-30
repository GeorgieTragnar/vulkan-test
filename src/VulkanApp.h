
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
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
	};


protected:

	VulkanApp();
	~VulkanApp();

	void createInstance();
	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();
	void setupDebugMessenger();
	VkResult createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo);
	void destroyDebugUtilsMessengerEXT();

	void checkExtensions();
	void pickPhysicalDevice();

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);


	const bool _enableValidationLayers;
	const std::vector<const char*> _validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	VkDebugUtilsMessengerEXT _debugMessenger;

	VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
	VkInstance _instance = nullptr;

};
