#pragma once
#include <iostream>
#include <optional>
#include <set>

#include "vulkanDebug.h"

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR& surface);

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& device, VkSurfaceKHR& surface);

bool checkDeviceExtensionSupport(VkPhysicalDevice& device);

bool isDeviceSuitable(VkPhysicalDevice& device, VkSurfaceKHR& surface);

// Different GPUs will have different memory types, and here we can find out what one we most want to use.
uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

std::vector<const char*> getRequiredExtensions(const bool enableValidationLayers);

// Returns desired format.  Formats are listed in order of preference.
VkFormat findDepthFormat(VkPhysicalDevice& physicalDevice);

// Used for multisampling.  Different GPUs will have different max samples/pixel.
// Here we just take the max amount of samples/pixel, and say fuck the framerate!
VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice& physicalDevice);