#include "GUIManager.h"

glm::vec2 GUIManager::windowRatioAdj = glm::vec2(1, 1);

bool GUIManager::Button::overButton(glm::vec2 screenPos) {
	float xPosAdj = pos.x * windowRatioAdj.x;
	float yPosAdj = pos.y * windowRatioAdj.y;
	pressed = xPosAdj< screenPos.x && xPosAdj + size.x > screenPos.x &&
		yPosAdj < screenPos.y&& yPosAdj + size.y > screenPos.y;

	return pressed;
}

void GUIManager::Layout::addButton(int type, glm::vec3 color) {
	Button newButton;
	buttons.push_back(newButton);
	float newBorderX = size.x * buttonBorderRatio;
}

void GUIManager::Layout::removeButton(int buttonIndex) {

}

GUIManager::GUIManager() {
	p_inputManager = nullptr;
	p_outVertices = nullptr;
	p_outIndices = nullptr;
	createLayouts();
}
void GUIManager::createLayouts() {
	Layout testLayout;
	testLayout.pos = glm::vec2(-1, 0.8f);
	testLayout.size = glm::vec2(2, 0.1f);
	testLayout.color = glm::vec3(0.1f, 0.1f, 0.5f);
	Button testButton;
	testButton.pos = glm::vec2(0.1f, 0.9f);
	testButton.size = glm::vec2(0.05f, 0.05f);
	testButton.color = glm::vec3(1, 0, 0);
	testLayout.buttons.push_back(testButton);
	testButton.pos = glm::vec2(0.2f, 0.9f);
	testButton.size = glm::vec2(0.05f, 0.05f);
	testLayout.buttons.push_back(testButton);

	layouts.push_back(testLayout);
}
void GUIManager::init(GLFWwindow* window, InputManager* inputManager, Camera* camera,
	std::vector<Vertex>* vertices, std::vector<uint32_t>* indices, std::vector <glm::mat4>* translationMatrices) {
	p_window = window;
	p_inputManager = inputManager;
	p_camera = camera;
	p_outVertices = vertices;
	p_outIndices = indices;
	p_outTranslationMatrices = translationMatrices;
}

void GUIManager::update() {
	windowRatioAdj = glm::vec2(1, 1);
	if (p_inputManager->currentWindowRatio > 1) {
		windowRatioAdj.x = 1 / p_inputManager->currentWindowRatio;
	}
	else {
		windowRatioAdj.y = p_inputManager->currentWindowRatio;
	}

	for (Layout& currentLayout : layouts) {
		for (Button& currentButton : currentLayout.buttons) {
			currentButton.overButton(p_inputManager->mouseScreenSpacePos);
			if (currentButton.pressed) {
			}
		}
	}

	pushDataToVulkan();
}
bool GUIManager::buttonState(int layoutIndex, int buttonIndex) {
	return false;
}
void GUIManager::pushDataToVulkan() {
	
	int matrixIndex = (int)p_outTranslationMatrices->size();
	p_outTranslationMatrices->push_back(p_camera->antiProjMatrix);
	Vertex temp;
	for (Layout& currentLayout : layouts) {
		for (Button& currentButton : currentLayout.buttons) {
			int indexOffset = (int)p_outVertices->size();
			p_outIndices->push_back(0 + indexOffset);
			p_outIndices->push_back(1 + indexOffset);
			p_outIndices->push_back(3 + indexOffset);
			p_outIndices->push_back(1 + indexOffset);
			p_outIndices->push_back(2 + indexOffset);
			p_outIndices->push_back(3 + indexOffset);

			temp.color = currentButton.color;
			temp.texCoord.x = 0;
			temp.texCoord.y = 0;
			temp.modelTranformMatrixIndex = matrixIndex;
			temp.pos = glm::vec3(currentButton.pos, 0);
			temp.pos.x *= windowRatioAdj.x;
			temp.pos.y *= windowRatioAdj.y;
			p_outVertices->push_back(temp);
			temp.pos.x += currentButton.size.x * windowRatioAdj.x;
			p_outVertices->push_back(temp);
			temp.pos.y += currentButton.size.y * windowRatioAdj.y;
			p_outVertices->push_back(temp);
			temp.pos.x -= currentButton.size.x * windowRatioAdj.x;
			p_outVertices->push_back(temp);
		}
		int indexOffset = (int)p_outVertices->size();
		p_outIndices->push_back(0 + indexOffset);
		p_outIndices->push_back(1 + indexOffset);
		p_outIndices->push_back(3 + indexOffset);
		p_outIndices->push_back(1 + indexOffset);
		p_outIndices->push_back(2 + indexOffset);
		p_outIndices->push_back(3 + indexOffset);
		temp.color = currentLayout.color;
		temp.texCoord.x = 0;
		temp.texCoord.y = 0;
		temp.modelTranformMatrixIndex = matrixIndex;
		temp.pos = glm::vec3(currentLayout.pos, 0);
		temp.pos.x *= windowRatioAdj.x;
		temp.pos.y *= windowRatioAdj.y;
		p_outVertices->push_back(temp);
		temp.pos.x += currentLayout.size.x * windowRatioAdj.x;
		p_outVertices->push_back(temp);
		temp.pos.y += currentLayout.size.y * windowRatioAdj.y;
		p_outVertices->push_back(temp);
		temp.pos.x -= currentLayout.size.x * windowRatioAdj.x;
		p_outVertices->push_back(temp);
	}
	// Letterboxing:
	temp.color = glm::vec3(.1f, .1f, .1f);
	temp.texCoord.x = 0;
	temp.texCoord.y = 0;
	temp.modelTranformMatrixIndex = matrixIndex;

	if (p_inputManager->currentWindowRatio > 1) {
		// Left side:
		int indexOffset = (int)p_outVertices->size();
		p_outIndices->push_back(0 + indexOffset);
		p_outIndices->push_back(1 + indexOffset);
		p_outIndices->push_back(3 + indexOffset);
		p_outIndices->push_back(1 + indexOffset);
		p_outIndices->push_back(2 + indexOffset);
		p_outIndices->push_back(3 + indexOffset);
		temp.pos = glm::vec3(-1, -1, 0);
		p_outVertices->push_back(temp);
		temp.pos = glm::vec3(-1 * windowRatioAdj.x, -1, 0);
		p_outVertices->push_back(temp);
		temp.pos = glm::vec3(-1 * windowRatioAdj.x, 1, 0);
		p_outVertices->push_back(temp);
		temp.pos = glm::vec3(-1, 1, 0);
		p_outVertices->push_back(temp);
		// Right side.	
		indexOffset = (int)p_outVertices->size();
		p_outIndices->push_back(0 + indexOffset);
		p_outIndices->push_back(1 + indexOffset);
		p_outIndices->push_back(3 + indexOffset);
		p_outIndices->push_back(1 + indexOffset);
		p_outIndices->push_back(2 + indexOffset);
		p_outIndices->push_back(3 + indexOffset);
		temp.pos = glm::vec3(1 * windowRatioAdj.x, -1, 0);
		p_outVertices->push_back(temp);
		temp.pos = glm::vec3(1, -1, 0);
		p_outVertices->push_back(temp);
		temp.pos = glm::vec3(1, 1, 0);
		p_outVertices->push_back(temp);
		temp.pos = glm::vec3(1 * windowRatioAdj.x, 1, 0);
		p_outVertices->push_back(temp);
	}
	else {
		// Top side:
		int indexOffset = (int)p_outVertices->size();
		p_outIndices->push_back(0 + indexOffset);
		p_outIndices->push_back(1 + indexOffset);
		p_outIndices->push_back(3 + indexOffset);
		p_outIndices->push_back(1 + indexOffset);
		p_outIndices->push_back(2 + indexOffset);
		p_outIndices->push_back(3 + indexOffset);
		temp.pos = glm::vec3(-1, -1, 0);
		p_outVertices->push_back(temp);
		temp.pos = glm::vec3(1, -1, 0);
		p_outVertices->push_back(temp);
		temp.pos = glm::vec3(1, -1 * windowRatioAdj.y, 0);
		p_outVertices->push_back(temp);
		temp.pos = glm::vec3(-1, -1 * windowRatioAdj.y, 0);
		p_outVertices->push_back(temp);
		// Bottom side.	
		indexOffset = (int)p_outVertices->size();
		p_outIndices->push_back(0 + indexOffset);
		p_outIndices->push_back(1 + indexOffset);
		p_outIndices->push_back(3 + indexOffset);
		p_outIndices->push_back(1 + indexOffset);
		p_outIndices->push_back(2 + indexOffset);
		p_outIndices->push_back(3 + indexOffset);
		temp.pos = glm::vec3(-1, 1 * windowRatioAdj.y, 0);
		p_outVertices->push_back(temp);
		temp.pos = glm::vec3(1, 1 * windowRatioAdj.y, 0);
		p_outVertices->push_back(temp);
		temp.pos = glm::vec3(1, 1, 0);
		p_outVertices->push_back(temp);
		temp.pos = glm::vec3(-1, 1, 0);
		p_outVertices->push_back(temp);
	}
}