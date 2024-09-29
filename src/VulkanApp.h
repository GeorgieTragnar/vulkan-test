
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
	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();
	void setupDebugMessenger();
	VkResult createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo);
	void destroyDebugUtilsMessengerEXT();

	void checkExtensions();


	const bool _enableValidationLayers;
	const std::vector<const char*> _validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	VkDebugUtilsMessengerEXT _debugMessenger;

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) 
	{
		if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
			LOG_INFO("validationlayer: {}", pCallbackData->pMessage);
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
			LOG_INFO("validationlayer: {}", pCallbackData->pMessage);
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			LOG_WARNING("validationlayer: {}", pCallbackData->pMessage);
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			LOG_ERROR("validationlayer: {}", pCallbackData->pMessage);
		else
			LOG_CRITICAL("unknown type bit: {}\nvalidationlayer: {}", int(messageSeverity), pCallbackData->pMessage);
			

		// TODO: messageType contains debug data info 

		// LOG_ERROR("validationlayer: {}", pCallbackData->pMessage);
		// std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	VkInstance _instance = nullptr;

};
