#pragma once
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> // Linear algebra.
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "VectorHelperFunctions.h"

extern float globalScale;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#define INPUT_MODE_MOVE 0
#define INPUT_MODE_PLACE 1

struct KeyBindings {
	int changeInputMode = GLFW_KEY_TAB;

	const int zoomIn = GLFW_KEY_EQUAL;
	const int zoomOut = GLFW_KEY_MINUS;

	const int goLeft = GLFW_KEY_A;
	const int goRight = GLFW_KEY_D;
	const int goForth = GLFW_KEY_W;
	const int goBack = GLFW_KEY_S;
	const int goUp = GLFW_KEY_SPACE;
	const int goDown = GLFW_KEY_LEFT_SHIFT;

	const int pitchUp = GLFW_KEY_UP;
	const int pitchDown = GLFW_KEY_DOWN;
	const int yawRight = GLFW_KEY_Q;
	const int yawLeft = GLFW_KEY_E;
	const int rollLeft = GLFW_KEY_LEFT;
	const int rollRight = GLFW_KEY_RIGHT;

	const int rotation = GLFW_MOUSE_BUTTON_RIGHT;
	const int translation = GLFW_MOUSE_BUTTON_LEFT;

	const int changePreviewOrientation = GLFW_KEY_R;
	const int changePreviewObjType = GLFW_KEY_T;
	const int changePreviewObjSubType = GLFW_KEY_G;
};

struct KeyInputLatch {
	bool click;
	bool pressed;
	int key;
	GLFWwindow* window;
	KeyInputLatch() : window(nullptr), key(0), click(false), pressed(false) {}

	void init(GLFWwindow* w, int GLFW_KEY) {
		window = w;
		key = GLFW_KEY;
	}
	void update() {
		click = false;
		if (glfwGetKey(window, key) && !pressed) {
			click = true;
			pressed = true;
		}
		if (glfwGetKey(window, key) == GLFW_RELEASE) {
			pressed = false;
		}
	}
};
struct MouseInputLatch {
	bool click;
	bool pressed;
	int button;
	GLFWwindow* window;
	MouseInputLatch() : window(nullptr), button(0), click(false), pressed(false) {}

	void init(GLFWwindow* w, int GLFW_KEY) {
		window = w;
		button = GLFW_KEY;
	}
	void update() {
		click = false;
		if (glfwGetMouseButton(window, button) && !pressed) {
			click = true;
			pressed = true;
		}
		if (glfwGetMouseButton(window, button) == GLFW_RELEASE) {
			pressed = false;
		}
	}
};

class InputManager {
public:
	GLFWwindow* p_window = nullptr;
	VkExtent2D* p_swapChainExtent = nullptr;
	int targetWindowRatio = 1;
	float currentWindowRatio;

	//float mouseX = 0, mouseY = 0;
	//float mouseLastClickedX = 0, mouseLastClickedY = 0;
	glm::vec2 mouseScreenPos;
	glm::vec2 mouseScreenSpacePos;
	glm::vec2 mousePlanePos;
	glm::vec2 mouseLastClickPos;
	glm::vec2 mouseDistFromLastClick;

	KeyBindings keyBindings;

	// Generic flags that can be keyed to any input in the update function:
	int inputMode = INPUT_MODE_MOVE;
	bool inputModeKeyPressed = false;
	// Camera:
	bool allowTranslation = false, allowRotation = false;
	bool zoomIn = false, zoomOut = false;
	bool goLeft = false, goRight = false, goForth = false, goBack = false, goUp = false, goDown = false;
	bool pitchDown = false, pitchUp = false;
	bool yawRight = false, yawLeft = false;
	bool rollLeft = false, rollRight = false;

	float zoomAdj = 0;

	// Buildings:
	bool allowBuildingPlacement = false;
	bool click = false;
	KeyInputLatch changePreviewBuildingOrientation;
	KeyInputLatch changePreviewObjType;
	KeyInputLatch changePreviewObjSubType;
	MouseInputLatch leftClick;
	MouseInputLatch rightClick;

private:



public:
	InputManager() {}

	void init(GLFWwindow& window, VkExtent2D& swapChainExtent, float windowRatio) {
		p_window = &window;
		p_swapChainExtent = &swapChainExtent;
		this->targetWindowRatio = int(windowRatio);
		//glfwSetScrollCallback(p_window, innerScrollCallback);
		glfwSetScrollCallback(p_window, scroll_callback);
		changePreviewBuildingOrientation.init(p_window, keyBindings.changePreviewOrientation);
		changePreviewObjType.init(p_window, keyBindings.changePreviewObjType);
		changePreviewObjSubType.init(p_window, keyBindings.changePreviewObjSubType);

		leftClick.init(p_window, GLFW_MOUSE_BUTTON_LEFT);
		rightClick.init(p_window, GLFW_MOUSE_BUTTON_RIGHT);
	}
	~InputManager() {}

	void setMouseLastClickPos() {
		double dx, dy;
		glfwGetCursorPos(p_window, &dx, &dy);
		mouseLastClickPos = glm::vec2(dx, dy);
	}

	bool pressed(int keyBinding) {
		return glfwGetKey(p_window, keyBinding);
	}

	void queryInputs() {
		click = false;

		changePreviewBuildingOrientation.update();
		changePreviewObjType.update();
		changePreviewObjSubType.update();

		leftClick.update();
		rightClick.update();

		if (pressed(keyBindings.changeInputMode) && inputModeKeyPressed == false) {
			inputModeKeyPressed = true;
			inputMode = (inputMode + 1) % 2;
			std::cout << inputMode << std::endl;
		}
		if (pressed(keyBindings.changeInputMode) == GLFW_RELEASE) {
			inputModeKeyPressed = false;
		}

		double dx, dy;
		glfwGetCursorPos(p_window, &dx, &dy);
		mouseScreenPos = glm::vec2(dx, dy);

		mouseScreenSpacePos = mouseScreenPos;
		int width, height;
		glfwGetWindowSize(p_window, &width, &height);
		mouseScreenSpacePos.x -= float(width) / 2;
		mouseScreenSpacePos.x /= float(width) / 2;
		mouseScreenSpacePos.y -= float(height) / 2;
		mouseScreenSpacePos.y /= float(height) / 2;

		currentWindowRatio = float(width) / float(height);

		double sx = 0, sy = 0;
		scroll_callback(p_window, sx, sy);

		// Scale:
		if (pressed(keyBindings.zoomIn)) {
			zoomIn = true;
		}
		else {
			zoomIn = false;
		}
		if (pressed(keyBindings.zoomOut)) {
			zoomOut = true;
		}
		else {
			zoomOut = false;
		}

		zoomAdj = globalScale;

		// Rotation:
		if (pressed(keyBindings.pitchUp)) {
			pitchUp = true;
		}
		else {
			pitchUp = false;
		}
		if (pressed(keyBindings.pitchDown)) {
			pitchDown = true;
		}
		else {
			pitchDown = false;
		}

		if (pressed(keyBindings.yawRight)) {
			yawRight = true;
		}
		else {
			yawRight = false;
		}
		if (pressed(keyBindings.yawLeft)) {
			yawLeft = true;
		}
		else {
			yawLeft = false;
		}

		if (inputMode == INPUT_MODE_MOVE) {
			if (glfwGetMouseButton(p_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && allowRotation == false) {
				allowRotation = true;
				setMouseLastClickPos();
			}
			if (allowRotation == true) {
				mouseDistFromLastClick = mouseScreenPos - mouseLastClickPos;
			}
			if (glfwGetMouseButton(p_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && allowRotation == true) {
				allowRotation = false;
			}
		}

		// Translation:
		if (pressed(keyBindings.goLeft)) {
			goLeft = true;
		}
		else {
			goLeft = false;
		}

		if (pressed(keyBindings.goRight)) {
			goRight = true;
		}
		else {
			goRight = false;
		}

		if (pressed(keyBindings.goForth)) {
			goForth = true;
		}
		else {
			goForth = false;
		}

		if (pressed(keyBindings.goBack)) {
			goBack = true;
		}
		else {
			goBack = false;
		}

		if (pressed(keyBindings.goUp)) {
			goUp = true;
		}
		else {
			goUp = false;
		}

		if (pressed(keyBindings.goDown)) {
			goDown = true;
		}
		else {
			goDown = false;
		}

		if (inputMode == INPUT_MODE_MOVE) {
			if (glfwGetMouseButton(p_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && allowTranslation == false) {
				allowTranslation = true;
				setMouseLastClickPos();
			}
			if (allowTranslation == true) {
				mouseDistFromLastClick = mouseScreenPos - mouseLastClickPos;
			}
			if (glfwGetMouseButton(p_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && allowTranslation == true) {
				if (getDist(mouseLastClickPos, mouseScreenPos) < 3.0f) {
					click = true;
				}
				allowTranslation = false;
			}
		}

		if (inputMode == INPUT_MODE_PLACE) {
			if (glfwGetMouseButton(p_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && allowBuildingPlacement == false) {
				allowBuildingPlacement = true;
			}
			if (glfwGetMouseButton(p_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && allowBuildingPlacement == true) {
				allowBuildingPlacement = false;
			}
		}
	}
};

