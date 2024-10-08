
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

	void drawFrame();
	void waitIdle();
	void recreateSwapChain();
	std::function<void()> getResizedCallback();

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

	const int MAX_FRAMES_IN_FLIGHT = 2;

	struct Vertex {
		glm::vec2 pos;
		glm::vec3 color;

		static VkVertexInputBindingDescription getBindingDescription() 
		{
			VkVertexInputBindingDescription bindingDescription{};

			bindingDescription.binding = 0;  // Binding index (usually 0)
			bindingDescription.stride = sizeof(Vertex);  // Size of one vertex
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;  // Per-vertex data

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

			// Position attribute
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;  // vec2 (x, y) as 2 floats
			attributeDescriptions[0].offset = offsetof(Vertex, pos);  // Offset of pos in Vertex

			// Color attribute
			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;  // vec3 (r, g, b) as 3 floats
			attributeDescriptions[1].offset = offsetof(Vertex, color);  // Offset of color in Vertex

			return attributeDescriptions;
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
	void createVertexBuffer();
	void createIndexBuffer();
	void createCommandBuffers();
	void createSyncObjects();

	void cleanupSwapChain();

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

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

	const std::vector<Vertex> _vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<uint16_t> _indices = {
		0, 1, 2, 2, 3, 0
	};

	VkDebugUtilsMessengerEXT _debugMessenger;

	std::atomic<bool> _frameBufferResized;
	uint32_t _currentFrame;

	std::vector<VkSemaphore> _imageAvailableSemaphores;
	std::vector<VkSemaphore> _renderFinishedSemaphores;
	std::vector<VkFence> _inFlightFences;
	
	VkDeviceMemory _indexBufferMemory;
	VkBuffer _indexBuffer;
	VkDeviceMemory _vertexBufferMemory;
	VkBuffer _vertexBuffer;
	std::vector<VkCommandBuffer> _commandBuffers;
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
	VkPhysicalDevice _physicalDevice;
	VkInstance _instance;

};
