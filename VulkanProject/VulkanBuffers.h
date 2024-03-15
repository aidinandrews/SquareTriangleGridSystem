#pragma once
#include <iostream>

#include "vulkanDebug.h"
#include "queryHardware.h"
#include "VulkanCommands.h"

void createBuffer(VkPhysicalDevice& physicalDevice, VkDevice& device,
	VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
	VkBuffer& buffer, VkDeviceMemory& bufferMemory);

void copyBuffer(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue,
	VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);