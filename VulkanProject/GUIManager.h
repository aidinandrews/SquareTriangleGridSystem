#pragma once
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "inputManager.h"
#include "CoordinateWrappers.h"
#include "Camera.h"

#define LAYOUT_TEST 0

#define BUTTON_BUILDING_TYPE_0 0
#define BUTTON_BUILDING_TYPE_1 1

class GUIManager {
	// internal structs:
public:

private:
	struct Button {
		glm::vec2 pos, size;
		glm::vec3 color;
		bool active;
		bool pressed;
		std::string name;
		Button() {
			pos = glm::vec2(0, 0);
			size = glm::vec2(0, 0);
			active = true;
			pressed = false;
			name = "none";
		}
		Button(glm::vec2 pos, glm::vec2 size, glm::vec3 color, std::string name) 
			: pos(pos), size(size), color(color), name(name) {
			pressed = false;
		}
		void move(glm::vec2 newPos) { pos = newPos; }
		void resize(glm::vec2 newSize) { size = newSize; }
		bool overButton(glm::vec2 screenPos);
	};
	struct Layout {
		glm::vec2 pos;
		glm::vec2 size;
		glm::vec3 color;
		std::string name;
		float buttonBorderRatio;
		bool active;
		std::vector<Button> buttons;
		void activate() { active = true; }
		void deactivate() { active = false; }

		void addButton(int type, glm::vec3 color);
		void removeButton(int buttonIndex);
	};
	// Variables:
public:
	GLFWwindow* p_window;
	InputManager* p_inputManager;
	Camera* p_camera;
	std::vector<Vertex>* p_outVertices;
	std::vector<uint32_t>* p_outIndices;
	std::vector <glm::mat4>* p_outTranslationMatrices;

	std::vector<Layout> layouts;
	static glm::vec2 windowRatioAdj;
private:

	// Functions:
public:
	GUIManager();

	void createLayouts();
	void init(GLFWwindow* window, InputManager* inputManager, Camera* camera,
		std::vector<Vertex>* vertices, std::vector<uint32_t>* indices, std::vector <glm::mat4>* translationMatrices);

	void update();

	bool buttonState(int layoutIndex, int buttonIndex);

	void pushDataToVulkan();
private:

};

