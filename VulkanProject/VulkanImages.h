#pragma once
#include <iostream>

#include "stbi_image.h"

#include "vulkanDebug.h"
#include "VulkanBuffers.h"
#include "VulkanCommands.h"
#include "queryHardware.h"

class ImageManager {
public:
	VkPhysicalDevice* p_physicalDevice;
	VkDevice* p_device;
	VkCommandPool* p_commandPool;
	VkQueue* p_graphicsQueue;

	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	std::vector<VkImageView> swapChainImageViews;

	VkImage greatWave;
	VkDeviceMemory greatWaveMemory;
	VkImageView greatWaveView;
	VkSampler greatWaveSampler;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	VkImage colorImage; // Used for multisampling (can store multiple colors/pixel then merge them)
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

public:
	ImageManager(): p_physicalDevice(VK_NULL_HANDLE), p_device(VK_NULL_HANDLE),
		p_commandPool(VK_NULL_HANDLE), p_graphicsQueue(VK_NULL_HANDLE) 
	{};

	void init(VkPhysicalDevice& physicalDevice, VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue) {
		p_physicalDevice = &physicalDevice;
		p_device = &device;
		p_commandPool = &commandPool;
		p_graphicsQueue = &graphicsQueue;
	}		

	void createResources() {
		createTextureImage(greatWave, greatWaveMemory, "textures/TheGreatWave.jpg");
		createTextureImageView(greatWaveView, greatWave);
		createTextureSampler(greatWaveSampler);
	}

	void cleanup() {
		vkDestroySampler(*p_device, greatWaveSampler, nullptr);
		vkDestroyImageView(*p_device, greatWaveView, nullptr);

		vkDestroyImage(*p_device, greatWave, nullptr);
		vkFreeMemory(*p_device, greatWaveMemory, nullptr);
	}

	~ImageManager();

	// Returns index of image for storage;
	int createImage(VkImage& textureImage, VkDeviceMemory& textureImageMemory, 
		uint32_t width, uint32_t height, VkSampleCountFlagBits numSamples, VkFormat format,
		VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);

	VkImageView createImageView(VkImage& image, VkFormat format, VkImageAspectFlags aspectFlags);
	void createTextureImage(VkImage& textureImage, VkDeviceMemory& textureImageMemory, const char* filePath);
	void createTextureImageView(VkImageView& textureImageView, VkImage& textureImage);
	void createTextureSampler(VkSampler& textureSampler);
	void transitionImageLayout(VkImage& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	// For every image inside Vulkan, there must be specified a way to access that image.
	// There are many ways to store data in an image. From RGB color data to floating point depth data.
	// The image view tells vulkan how to interperate the data it gets from the image.
	void createSwapChainImageViews();
	void createColorResources(VkExtent2D swapChainExtent);
	void createDepthResources(VkExtent2D swapChainExtent);


private:
	void copyBufferToImage(VkBuffer& buffer, VkImage& image, uint32_t& width, uint32_t& height);
};