#include "LineSegmentObject.h"

LineSegObj::LineSegObj() {
	orientationFlag = 0;
	xxyys[0] = XXYY(0, 0, 0, 0);
	xxyys[1] = XXYY(4, 0, 0, 0);
	updateSpine();
	model = nullptr;
	color = glm::vec3(1, 1, 1);
	worldTransMatrix = glm::mat4(1);
}
void LineSegObj::updateSpine() {
	spine[0] = xxyys[0].getXYpos();
	spine[4] = xxyys[1].getXYpos();
	spine[2].x = (spine[0].x + spine[4].x) / 2.0f;
	spine[2].y = (spine[0].y + spine[4].y) / 2.0f;
	spine[1].x = (spine[0].x + spine[2].x) / 2.0f;
	spine[1].y = (spine[0].y + spine[2].y) / 2.0f;
	spine[3].x = (spine[2].x + spine[4].x) / 2.0f;
	spine[3].y = (spine[2].y + spine[4].y) / 2.0f;
}
void LineSegObj::setColor(float r, float g, float b) {
	color = glm::vec3(r, g, b);
}
void LineSegObj::move(XXYY adj) {
	xxyys[0] += adj;
	xxyys[1] += adj;
	updateSpine();
	worldTransMatrix[3][0] = spine[2].x;
	worldTransMatrix[3][1] = spine[2].y;
}
void LineSegObj::setPos(int xxyyIndex, XXYY newPos) {
	XXYY adj = newPos - xxyys[xxyyIndex];
	xxyys[0] += adj;
	xxyys[1] += adj;
	updateSpine();
	worldTransMatrix[3][0] = xxyys[0].getXYpos().x;
	worldTransMatrix[3][1] = xxyys[0].getXYpos().y;
}
void LineSegObj::rotate(int xxyyIndex, const int ROTATE_FLAG) {
	orientationFlag = (orientationFlag + ROTATE_FLAG) % 12;
	setRotation();

	// Rotate the xxyy points by referencing the only 12 options from one pos to another.
	XXYY xxyyUnitVector = xxyys[(xxyyIndex + 1) % 2] - xxyys[xxyyIndex];
	XXYY newXxyyUnitVector = getNextAdj(xxyyUnitVector);
	xxyys[(xxyyIndex + 1) % 2] = xxyys[xxyyIndex] + newXxyyUnitVector;
	updateSpine();
}
void LineSegObj::setRotation() {
	float r = getRadians(orientationFlag);
	worldTransMatrix[0][0] = cos(r);
	worldTransMatrix[0][1] = -sin(r);
	worldTransMatrix[1][0] = sin(r);
	worldTransMatrix[1][1] = cos(r);
}

void LineSegObj::pushDataToVulkan(Camera& camera, std::vector<Vertex>* outVertices, std::vector<uint32_t>* outIndices, std::vector<glm::mat4>* outTranslationMatrices) {
	int transMatrixIndex = (int)outTranslationMatrices->size();
	glm::mat4 scaleMatrix(1);
	float z = 1 / float(pow(2, camera.zoom + camera.p_inputManager->zoomAdj));
	scaleMatrix[1][1] = z;
	scaleMatrix[2][2] = z;
	outTranslationMatrices->push_back(worldTransMatrix * scaleMatrix);

	int indexOffset = (int)outVertices->size();
	// Preview object fist so its on top:
	for (int i = 0; i < model->indices.size(); i++) {
		outIndices->push_back(model->indices[i] + indexOffset);
	}
	for (int v = 0; v < model->verts.size(); v++) {
		Vertex temp = model->verts[v];
		temp.color = color;
		temp.modelTranformMatrixIndex = transMatrixIndex;
		outVertices->push_back(temp);
		indexOffset++;
	}
}

void LineSegObj::pushBeltDataToVulkan(Camera& camera, std::vector<Vertex>* outVertices, std::vector<uint32_t>* outIndices, 
	std::vector<glm::mat4>* outTranslationMatrices, float* gameStepPercent) {
	int transMatrixIndex = (int)outTranslationMatrices->size();
	glm::mat4 scaleMatrix(1);
	float z = 1 / float(pow(2, camera.zoom + camera.p_inputManager->zoomAdj));
	scaleMatrix[1][1] = z;
	scaleMatrix[2][2] = z;
	outTranslationMatrices->push_back(worldTransMatrix * scaleMatrix);

	int indexOffset = (int)outVertices->size();
	// Preview object fist so its on top:
	for (int i = 0; i < model->indices.size(); i++) {
		outIndices->push_back(model->indices[i] + indexOffset);
	}
	for (int v = 0; v < model->verts.size(); v++) {
		Vertex temp = model->verts[v];
		temp.color = color;
		temp.modelTranformMatrixIndex = transMatrixIndex;
		outVertices->push_back(temp);
		indexOffset++;
	}

	glm::vec2 modelTransSteps = ((xxyys[1].getXYpos() - xxyys[0].getXYpos()) / 4.0f);
	float adj = (sin((*gameStepPercent * M_PI) - (M_PI / 2)) + 1) / 2;
	glm::vec2 modelTransStepsAdj = modelTransSteps * adj;
	glm::vec2 worldPos = glm::vec2(worldTransMatrix[3][0], worldTransMatrix[3][1]);
	glm::mat4 arrowTransMatrix = worldTransMatrix;
	float s = 3;
	if (s > z) { s = z; }
	scaleMatrix = {
		s, 0, 0, 0,
		0, s, 0, 0,
		0, 0, s, 0,
		0, 0, 0, 1
	};
	s = *gameStepPercent;
	glm::mat4 firstArrowScaleIn = {
		s, 0, 0, 0,
		0, s, 0, 0,
		0, 0, s, 0,
		0, 0, 0, 1
	};
	s = 1 - *gameStepPercent;
	glm::mat4 lastArrowScaleOut = {
		s, 0, 0, 0,
		0, s, 0, 0,
		0, 0, s, 0,
		0, 0, 0, 1
	};

	// first arrow that pops into existance:
	transMatrixIndex = (int)outTranslationMatrices->size();
	outTranslationMatrices->push_back(worldTransMatrix * firstArrowScaleIn * scaleMatrix);

	indexOffset = (int)outVertices->size();
	for (int i = 0; i < BELT_ARROW.indices.size(); i++) {
		outIndices->push_back(BELT_ARROW.indices[i] + indexOffset);
	}
	for (int v = 0; v < BELT_ARROW.verts.size(); v++) {
		Vertex temp = BELT_ARROW.verts[v];
		temp.color = color;
		temp.modelTranformMatrixIndex = transMatrixIndex;
		outVertices->push_back(temp);
		indexOffset++;
	}

	for (int arrow = 0; arrow < 4; arrow++) { // middle arrows that move:
		int transMatrixIndex = (int)outTranslationMatrices->size();
		arrowTransMatrix[3][0] = modelTransSteps.x * arrow + modelTransStepsAdj.x + worldPos.x;
		arrowTransMatrix[3][1] = modelTransSteps.y * arrow + modelTransStepsAdj.y + worldPos.y;
		outTranslationMatrices->push_back(arrowTransMatrix * scaleMatrix);

		int indexOffset = (int)outVertices->size();
		for (int i = 0; i < BELT_ARROW.indices.size(); i++) {
			outIndices->push_back(BELT_ARROW.indices[i] + indexOffset);
		}
		for (int v = 0; v < BELT_ARROW.verts.size(); v++) {
			Vertex temp = BELT_ARROW.verts[v];
			temp.color = color;
			temp.modelTranformMatrixIndex = transMatrixIndex;
			outVertices->push_back(temp);
			indexOffset++;
		}	
	}

	// last arrow that eeps out of existance:
	transMatrixIndex = (int)outTranslationMatrices->size();
	arrowTransMatrix[3][0] = modelTransSteps.x * 4 + worldPos.x;
	arrowTransMatrix[3][1] = modelTransSteps.y * 4 + worldPos.y;
	outTranslationMatrices->push_back(arrowTransMatrix * lastArrowScaleOut * scaleMatrix);

	indexOffset = (int)outVertices->size();
	for (int i = 0; i < BELT_ARROW.indices.size(); i++) {
		outIndices->push_back(BELT_ARROW.indices[i] + indexOffset);
	}
	for (int v = 0; v < BELT_ARROW.verts.size(); v++) {
		Vertex temp = BELT_ARROW.verts[v];
		temp.color = color;
		temp.modelTranformMatrixIndex = transMatrixIndex;
		outVertices->push_back(temp);
		indexOffset++;
	}
}
