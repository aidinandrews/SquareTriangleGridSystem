#pragma once
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <chrono>

#include "inputManager.h"
#include "vectorHelperFunctions.h"

extern glm::mat4 identity;
extern glm::vec2 Mouse_Plane_Pos;

class Camera {
public:
	glm::vec3 cursorPlanePos;

	glm::vec3 worldPos;
	glm::vec3 planePos;
	glm::vec3 planePosAdj;

	glm::mat4 projMatrix;
	glm::mat4 antiProjMatrix;

	glm::vec2 windowAdj;
	glm::vec2 mousePlanePos;

	float zoom = -2.0f;
	float yaw = 0, yawAdj = 0;
	float pitch = 0, pitchAdj = 0;
	float roll = 0, rollAdj = 0;

	bool allowRotationAdj = false;
	bool allowTranslationAdj = false;
	InputManager* p_inputManager;
private:
	GLFWwindow* p_window;


public:
	Camera() {
		worldPos = glm::vec3(0, 0, 0);
		planePos = glm::vec3(0, 0, 0);
		planePosAdj = glm::vec3(0, 0, 0);
		windowAdj = glm::vec2(1, 1);
	}
	~Camera() {}

	void init(GLFWwindow& window, InputManager& inputManager) {
		p_window = &window;
		p_inputManager = &inputManager;
	}

	float zoomRatio() {
		return float(1 / pow(2, zoom));
	}

	void updateViaInputs() {
		// Scale:
		if (p_inputManager->zoomIn) { zoom += 0.1f; }
		if (p_inputManager->zoomOut) { zoom -= 0.1f; }

		// Rotation:
		if (p_inputManager->pitchUp) { pitch += 0.1f; }
		if (p_inputManager->pitchDown) { pitch -= 0.1f; }

		if (p_inputManager->yawRight) { yaw += 0.1f; }
		if (p_inputManager->yawLeft) { yaw -= 0.1f; }

		if (p_inputManager->rollRight) { roll += 0.1f; }
		if (p_inputManager->rollLeft) { roll -= 0.1f; }

		// Translation:
		glm::vec3 adj(0, 0, 0);
		float p = (yaw + yawAdj);
		if (p_inputManager->goLeft) { glm::vec3 t(0.01f, 0, 0); rotate(t, -p); adj += t; }
		if (p_inputManager->goRight) { glm::vec3 t(-0.01f, 0, 0); rotate(t, -p); adj += t; }
		if (p_inputManager->goForth) { glm::vec3 t(0, 0.01f, 0); rotate(t, -p); adj += t; }
		if (p_inputManager->goBack) { glm::vec3 t(0, -0.01f, 0); rotate(t, -p); adj += t; }
		if (p_inputManager->goUp) { adj.z += 0.01f; }
		if (p_inputManager->goDown) { adj.z -= 0.01f; }
		adj *= 1 / pow(2, zoom);
		adj *= 3;
		planePos += adj;

		// Mouse:
		if (p_inputManager->allowRotation == true && allowRotationAdj == false) {
			allowRotationAdj = true;
		}
		if (allowRotationAdj) {
			yawAdj = -(p_inputManager->mouseDistFromLastClick.x) * 0.005f;
			pitchAdj = (p_inputManager->mouseDistFromLastClick.y) * 0.005f;
		}
		if (p_inputManager->allowRotation == false && allowRotationAdj == true) {
			allowRotationAdj = false;
			yaw += yawAdj;
			pitch += pitchAdj;
			yawAdj = 0;
			pitchAdj = 0;
		}

		// Mouse:
		if (p_inputManager->allowTranslation == true && allowTranslationAdj == false) {
			allowTranslationAdj = true;
		}
		if (allowTranslationAdj == true) {
			int w, h;
			float adjx, adjy;
			glfwGetWindowSize(p_window, &w, &h);
			if (w > h) {
				adjx = (p_inputManager->mouseDistFromLastClick.x / h * p_inputManager->targetWindowRatio) * 2;
			}
			else {
				adjx = (p_inputManager->mouseDistFromLastClick.x / w) * 2;
			}
			adjy = (p_inputManager->mouseDistFromLastClick.y / h) * 2;
			glm::vec3 t(adjx, adjy, 0);
			rotate(t, -yaw - yawAdj);
			t /= float(pow(2, zoom + p_inputManager->zoomAdj));
			planePosAdj = t;
		}
		if (p_inputManager->allowTranslation == false && allowTranslationAdj == true) {
			allowTranslationAdj = false;
			planePos += planePosAdj;
			planePosAdj = glm::vec3(0, 0, 0);
		}
	}

	void update() {
		updateViaInputs();

		int wi, he;
		glfwGetWindowSize(p_window, &wi, &he);
		float w = float(wi), h = float(he);
		if (w / h > p_inputManager->targetWindowRatio) { windowAdj.x = p_inputManager->targetWindowRatio * (h / w); }
		else { windowAdj.y = (1 / p_inputManager->targetWindowRatio) * (w / h); }

		getProjectionMatrix();
		getPlanePosFromScreenPos();
	};

	glm::mat4 getProjectionMatrix() {
		glm::vec3 t = (planePos + planePosAdj);
		float z = 1 / float(pow(2, zoom + p_inputManager->zoomAdj));

		glm::mat4 translate = glm::translate(glm::mat4(1), glm::vec3(-t.x, t.y, 0));
		glm::mat4 rotateZ = glm::rotate(glm::mat4(1), -yaw - yawAdj, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 rotateX = glm::rotate(glm::mat4(1), -pitch - pitchAdj, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 model = rotateX * rotateZ * translate;

		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.01f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		//proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 proj = glm::ortho(
			-1.0f / windowAdj.x * z, 1.0f / windowAdj.x * z,
			-1.0f * windowAdj.y * z, 1.0f * windowAdj.y * z,
			-100.0f, 100.0f);
		proj[1][1] *= -1;
		projMatrix = proj * view * model;

		return projMatrix;
	}

	glm::vec2 getPlanePosFromScreenPos() {
		float z = 1 / float(pow(2, zoom + p_inputManager->zoomAdj));
		glm::vec3 t = (planePos + planePosAdj);

		glm::vec4 transitoryCursorPos = glm::vec4(0, 0, 0, 1);
		glm::vec3 cursorAdj = glm::vec3(p_inputManager->mouseScreenSpacePos, 0);
		cursorAdj.x /= windowAdj.x;
		cursorAdj.y /= windowAdj.y;



		glm::vec3 translation = glm::vec3( // Boy howdy am I going to forget why this works...
			(t.x * (1 / z)) - cos(yaw + yawAdj) * cursorAdj.x - sin(yaw + yawAdj) * cursorAdj.y * (1 / cos(pitch + pitchAdj)),
			(-t.y * (1 / z)) - sin(yaw + yawAdj) * cursorAdj.x + cos(yaw + yawAdj) * cursorAdj.y * (1 / cos(pitch + pitchAdj)),
			0);
		translation.x *= z;
		translation.y *= z;
		transitoryCursorPos = glm::translate(glm::mat4(1), translation) * transitoryCursorPos;

		cursorPlanePos = transitoryCursorPos;
		return glm::vec2(-cursorPlanePos.x, cursorPlanePos.y);
	}
};

inline glm::vec2 getPlanePos(GLFWwindow* p_window, InputManager* p_inputManager, Camera* p_camera, glm::vec2 screenPos) {
	glm::vec3 mousePos(screenPos, 0);
	// pixel space -> window space:
	int w, h;
	glfwGetWindowSize(p_window, &w, &h);
	float width = float(w);
	float height = float(h);
	if (width > height) {
		mousePos.x = (screenPos.x - (width / 2)) / ((height * p_inputManager->targetWindowRatio) / 2);
		mousePos.y = (screenPos.y - (height / 2)) / (height / 2);
	}
	else {
		mousePos.x = (screenPos.x - (width / 2)) / (width / 2);
		mousePos.y = (screenPos.y - (height / 2)) / ((width * (1 / p_inputManager->targetWindowRatio)) / 2);
	}
	// De-zoomify:
	mousePos /= float(pow(2, p_camera->zoom + p_inputManager->zoomAdj));
	// De-yawify:
	mousePos.y /= cos(p_camera->pitch + p_camera->pitchAdj);
	// De-pitch:
	float r = -(p_camera->yaw + p_camera->yawAdj);
	rotate(mousePos, r);
	// De-translate it:
	glm::vec3 t = (p_camera->planePos + p_camera->planePosAdj) / float(pow(2, p_camera->zoom + p_inputManager->zoomAdj));
	mousePos -= t;

	return glm::vec2(mousePos.x, mousePos.y);
}