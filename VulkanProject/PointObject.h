#pragma once
#include <iostream>

#include "CoordinateWrappers.h"
#include "Model.h"
#include "Camera.h"

class PointObj {
public:
	int orientationFlag;
	XXYY xxyy;
	Model* model;
	glm::vec3 color;
	glm::vec2 xy;
	glm::mat4 worldTransMatrix;

public:
	PointObj();

	void setColor(float r, float g, float b);
	void move(XXYY adj);
	void setPos(XXYY newPos);
	void rotate(int rotationFlag);

	void pushDataToVulkan(Camera& camera, std::vector<Vertex>* outVertices, std::vector<uint32_t>* outIndices, std::vector <glm::mat4>* outTranslationMatrices);
};