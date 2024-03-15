#pragma once
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> // Linear algebra.
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

inline float getDist(glm::vec3 A, glm::vec3 B) {
	return sqrt(((B.x - A.x) * (B.x - A.x)) + ((B.y - A.y) * (B.y - A.y)) + ((B.z - A.z) * (B.z - A.z)));
}

inline float getDist(glm::vec2 A, glm::vec2 B) {
	return sqrt(((B.x - A.x) * (B.x - A.x)) + ((B.y - A.y) * (B.y - A.y)));
}

inline void rotate(glm::vec3& point, float radians) {
	glm::vec2 temp;
	temp.x = (point.x * cos(radians)) + (point.y * -sin(radians));
	temp.y = (point.x * sin(radians)) + (point.y * cos(radians));
	point = glm::vec3(temp.x, temp.y, 0);
}

inline void rotate(glm::vec3& point, glm::vec3 center, float radians) {
	glm::vec2 temp;
	point -= center;
	temp.x = (point.x * cos(radians)) + (point.y * -sin(radians));
	temp.y = (point.x * sin(radians)) + (point.y * cos(radians));
	point = glm::vec3(temp.x, temp.y, 0);
	point += center;
}

inline void rotate(glm::vec2& point, glm::vec2 center, float radians) {
	glm::vec2 temp;
	point -= center;
	temp.x = (point.x * cos(radians)) + (point.y * -sin(radians));
	temp.y = (point.x * sin(radians)) + (point.y * cos(radians));
	point = glm::vec2(temp.x, temp.y);
	point += center;
}

inline glm::vec3 randColor() {
	return glm::vec3(1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX, 1.0f * rand() / RAND_MAX);
}