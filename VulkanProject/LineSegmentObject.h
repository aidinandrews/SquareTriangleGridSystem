#pragma once
#include <iostream>

#include "CoordinateWrappers.h"
#include "Model.h"
#include "Camera.h"

const Model BELT_ARROW("models/CircleNode.obj");

class LineSegObj {
public:
	XXYY xxyys[2];
	glm::vec2 spine[5];
	int orientationFlag;
	Model* model;
	glm::vec3 color;
	glm::mat4 worldTransMatrix;

public:
	LineSegObj();

	void updateSpine();

	void setColor(float r, float g, float b);
	void move(XXYY adj);
	void setPos(int xxyyIndex, XXYY newPos);
	void rotate(int xxyyIndex, const int ROTATE_FLAG);
	void setRotation();

	void pushDataToVulkan(Camera& camera, std::vector<Vertex>* outVertices,
		std::vector<uint32_t>* outIndices, std::vector <glm::mat4>* outTranslationMatrices);
	void pushBeltDataToVulkan(Camera& camera, std::vector<Vertex>* outVertices,
		std::vector<uint32_t>* outIndices, std::vector <glm::mat4>* outTranslationMatrices, float* gameStepPercent);
};

