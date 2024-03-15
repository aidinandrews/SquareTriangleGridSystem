#pragma once
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>
#include <vector>

#include "inputManager.h"
#include "GUIManager.h"
#include "Camera.h"
#include "VectorHelperFunctions.h"
#include "CoordinateWrappers.h"

#define ENTITY_TYPE_TEST 0

class Entity {
	glm::vec3 pos;
	glm::vec3 orientation;
	int type;
};

