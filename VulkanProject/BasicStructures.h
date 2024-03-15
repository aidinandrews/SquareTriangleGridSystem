#pragma once
#include <iostream>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLFW/glfw3.h>

#include "queryHardware.h"

// A Vulkan instance is the connection between the application and the Vulkan Library.
void createInstance(VkInstance& instance);

// Because we are using GLFW, we need to make a GLFW window surface.
	// This is basically a screen that we can push information to and then present on the monitor.
void createSurface(VkInstance& instance, GLFWwindow& window, VkSurfaceKHR& surface);

// We must now explicitely pick a physical device (GPU) that vulkan will interact with in order to process image data.
void pickPhysicalDevice(VkInstance& instance, VkSurfaceKHR& surface, 
	VkPhysicalDevice& physicalDevice, VkSampleCountFlagBits& msaaSamples);

// A logical device is how Vulkan interacts with the GPU specified in the physical device structure.
void createLogicalDevice(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, 
	VkDevice& device, VkQueue& graphicsQueue, VkQueue& presentQueue);