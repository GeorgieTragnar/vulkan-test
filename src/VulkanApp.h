
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
	void getResolution(int32_t& width, int32_t& height);

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
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;



		static VkVertexInputBindingDescription getBindingDescription() 
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() 
		{
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

			return attributeDescriptions;
		}

		bool operator==(const Vertex& other) const 
		{
			return pos == other.pos && color == other.color && texCoord == other.texCoord;
		}
	};
	friend struct std::hash<Vertex>;

	struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
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
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createFrameBuffers();
	void createCommandPool();
	void createColorResources();
	void createDepthResources();
    void loadModel();
	void createTextureImage();
	void createTextureImageView();
	void createTextureSampler();
	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();
	void createCommandBuffers();
	void createSyncObjects();

	void cleanupSwapChain();

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void updateUniformBuffer(uint32_t currentImage);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, 
		VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, 
		VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	VkShaderModule createShaderModule(const std::vector<char>& code);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	VkSampleCountFlagBits getMaxUsableSampleCount();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);


	const bool						_enableValidationLayers;
	const std::vector<const char*>	_validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*>	_deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	const std::string				MODEL_PATH = "models/viking_room.obj";
	const std::string				TEXTURE_PATH = "textures/viking_room.png";

	std::vector<Vertex>				_vertices;
	std::vector<uint32_t>			_indices;

	VkDebugUtilsMessengerEXT		_debugMessenger;

	std::atomic<bool>				_frameBufferResized;
	uint32_t						_currentFrame;

	std::vector<VkSemaphore>		_imageAvailableSemaphores;
	std::vector<VkSemaphore>		_renderFinishedSemaphores;
	std::vector<VkFence>			_inFlightFences;
	
	VkImage							_colorImage;
	VkDeviceMemory					_colorImageMemory;
	VkImageView						_colorImageView;
	VkSampleCountFlagBits 			_msaaSamples;
	VkImage 						_depthImage;
	VkDeviceMemory 					_depthImageMemory;
	VkImageView 					_depthImageView;
	uint32_t						_textureMipLevels;
	VkSampler						_textureSampler;
	VkImageView						_textureImageView;
	VkImage							_textureImage;
	VkDeviceMemory					_textureImageMemory;
	VkBuffer						_stagingBuffer;
	VkDeviceMemory					_stagingBufferMemory;
	VkDescriptorPool				_descriptorPool;
	std::vector<VkDescriptorSet>	_descriptorSets;
	VkDescriptorSetLayout			_descriptorSetLayout;
	std::vector<VkBuffer>			_uniformBuffers;
	std::vector<VkDeviceMemory>		_uniformBuffersMemory;
	std::vector<void*>				_uniformBuffersMapped;
	VkDeviceMemory					_indexBufferMemory;
	VkBuffer						_indexBuffer;
	VkDeviceMemory					_vertexBufferMemory;
	VkBuffer						_vertexBuffer;
	std::vector<VkCommandBuffer>	_commandBuffers;
	VkCommandPool					_commandPool;
	std::vector<VkFramebuffer>		_swapChainFramebuffers;
	VkPipeline						_graphicsPipeline;
	VkRenderPass					_renderPass;
	VkPipelineLayout				_pipelineLayout;
	std::vector<VkImageView>		_swapChainImageViews;
	std::vector<VkImage>			_swapChainImages;
	VkSwapchainKHR					_swapChain;
	VkFormat						_swapChainImageFormat;
	VkExtent2D						_swapChainExtent;
	VkQueue							_presentQueue;
	VkSurfaceKHR					_surface;
	VkQueue							_graphicsQueue;
	VkPhysicalDeviceFeatures		_deviceFeatures{};
	VkDevice						_device;
	VkPhysicalDevice				_physicalDevice;
	VkInstance						_instance;

};
