#pragma once
#include <iostream>

#include "vulkanDebug.h"
#include "queryHardware.h"

// A command pool allocated the memory for a command buffer, 
// which is the structure used to tell the GPU what we want it to do!
void createCommandPool(VkPhysicalDevice& physicalDevice, VkDevice& device, VkCommandPool& commandPool, VkSurfaceKHR& surface);

// Helper functions for copying buffers:
VkCommandBuffer beginSingleTimeCommands(VkDevice& device, VkCommandPool& commandPool);

void endSingleTimeCommands(VkDevice& device, VkCommandPool& commandPool, VkCommandBuffer commandBuffer, VkQueue graphicsQueue);