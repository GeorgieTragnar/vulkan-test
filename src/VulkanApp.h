
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

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR _capabilities;
		std::vector<VkSurfaceFormatKHR> _formats;
		std::vector<VkPresentModeKHR> _presentModes;
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
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	void createSurface();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createGraphicsPipeline();
	void createFrameBuffers();
	void createCommandPool();
	void createCommandBuffer();

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	VkShaderModule createShaderModule(const std::vector<char>& code);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);


	const bool _enableValidationLayers;
	const std::vector<const char*> _validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> _deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkDebugUtilsMessengerEXT _debugMessenger;

	VkCommandBuffer _commandBuffer;
	VkCommandPool _commandPool;
	std::vector<VkFramebuffer> _swapChainFramebuffers;
	VkPipeline _graphicsPipeline;
	VkRenderPass _renderPass;
	VkPipelineLayout _pipelineLayout;
	std::vector<VkImageView> _swapChainImageViews;
	std::vector<VkImage> _swapChainImages;
	VkSwapchainKHR _swapChain;
	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;
	VkQueue _presentQueue;
	VkSurfaceKHR _surface;
	VkQueue _graphicsQueue;
	VkPhysicalDeviceFeatures _deviceFeatures{};
	VkDevice _device;
	VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
	VkInstance _instance = nullptr;

};
