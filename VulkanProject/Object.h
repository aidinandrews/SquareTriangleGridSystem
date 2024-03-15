#pragma once
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#ifndef TINY_OBJ_LOADED
#include "tiny_obj_loader.h"
#endif

#include "inputManager.h"
#include "GUIManager.h"
#include "Camera.h"
#include "VectorHelperFunctions.h"
#include "CoordinateWrappers.h"
#include "Entity.h"
#include "Model.h"

#define OBJ_TYPE_TRIANGLE 0
#define OBJ_TYPE_SQUARE   1

class ObjManager {
public:
	struct Obj {
		Model* model; // not used for now but after xxyy check, push these verts to vulkan!
		int orientationFlag;

		glm::vec3 color;
		std::vector<XXYY> xxyys;
		glm::vec2 center;
		glm::mat4 worldTransMatrix;

		// Inputs and outputs are defined by their index into the xxyys vector.
		// Each input/output is a line segment spanning from one xxyy to the next xxyy in the vector.
		// Ex: Input of index [3] is the line segment from xxyys[3] to xxyys[4]
		std::vector<int> inputIndices;
		std::vector<int> outputIndices;

		std::vector<Entity> storedEntities;

		Obj();
		~Obj();

		void setColor(float r, float g, float b);
		void resetCenter();
		void move(XXYY adj);
		void setPos(int anchorIndex, XXYY newPos);
		void rotate(int anchorIndex, const int ROTATE_FLAG);
		void setRotation();
		
		// Returns 0 if there is a connection and there is no overlap,
		// 1 if there is a connection and there is overlap,
		// and 2 if there is no connection and no overlap.
		bool BoundingBoxoverlaps(Obj& other);
		bool overlaps(Obj& other);

		void pushDataToVulkan(std::vector<Vertex>* outVertices, std::vector<uint16_t>* outIndices, std::vector <glm::mat4>* outTranslationMatrices);
	};
private:
	GLFWwindow* p_window;
	InputManager* p_inputManager;
	GUIManager* p_guiManager;
	std::vector<Vertex>* p_outVertices;
	std::vector<uint16_t>* p_outIndices;
	std::vector <glm::mat4>* p_outTranslationMatrices;
public:
	Camera* p_camera;
	std::vector<Model> m_models;
	// Objects are described structurally by their outline.
	// An outline must be listed clockwise!
	std::vector<Obj> m_baseObjs;

	std::vector<Obj> m_objs;
	Obj m_previewObj;
	int m_previewObjBaseIndex;
private:

public:
	ObjManager();
	void defineModels(std::vector<Model>& models);
	void defineBaseObjects(std::vector<Obj>& baseObjs);
	void init(GLFWwindow* window, InputManager* inputManager, GUIManager* guiManager, Camera* camera,
		std::vector<Vertex>* vertices, std::vector<uint16_t>* indices, std::vector <glm::mat4>* objMatrices);

	void update();

	bool tryToSnapToNearbyObj();
	bool tryToSnapPointToWorld();
	bool tryToDeleteObject();

	void pushDataToVulkan();
};