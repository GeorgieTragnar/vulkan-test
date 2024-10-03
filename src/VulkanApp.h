
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
		std::optional<uint32_t> _graphicsFamily;
		std::optional<uint32_t> _presentFamily;

		bool isComplete() {
			return _graphicsFamily.has_value() && _presentFamily.has_value();
		}
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
	void createLogicalDevice();

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);


	const bool _enableValidationLayers;
	const std::vector<const char*> _validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	VkDebugUtilsMessengerEXT _debugMessenger;

	VkQueue _presentQueue;
	VkSurfaceKHR _surface;
	VkQueue _graphicsQueue;
	VkPhysicalDeviceFeatures _deviceFeatures{};
	VkDevice _device;
	VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
	VkInstance _instance = nullptr;

};
