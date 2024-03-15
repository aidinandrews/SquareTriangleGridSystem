#include "PointObject.h"

PointObj::PointObj() {
	orientationFlag = ORIENTATION_0;
	xxyy = XXYY(0, 0, 0, 0);
	model = nullptr;
	color = glm::vec3(1, 1, 1);
	xy = glm::vec2(0, 0);
	worldTransMatrix = glm::mat4(1);
}
void PointObj::setColor(float r, float g, float b) {
	color = glm::vec3(r, g, b);
}
void PointObj::move(XXYY adj) {
	xxyy += adj;
	xy = xxyy.getXYpos();
	worldTransMatrix[3][0] = xy.x;
	worldTransMatrix[3][1] = xy.y;
}
void PointObj::setPos(XXYY newPos) {
	xxyy = newPos;
	xy = xxyy.getXYpos();
	worldTransMatrix[3][0] = xy.x;
	worldTransMatrix[3][1] = xy.y;
}

void PointObj::rotate(int orientationFlag) {
	float r = getRadians(orientationFlag);
	worldTransMatrix[0][0] = cos(r);
	worldTransMatrix[0][1] = -sin(r);
	worldTransMatrix[1][0] = sin(r);
	worldTransMatrix[1][1] = cos(r);
}

void PointObj::pushDataToVulkan(Camera& camera, std::vector<Vertex>* outVertices, std::vector<uint32_t>* outIndices, std::vector<glm::mat4>* outTranslationMatrices) {
	int transMatrixIndex = (int)outTranslationMatrices->size();
	glm::mat4 scaleMatrix(1);
	float z = 1 / float(pow(2, camera.zoom + camera.p_inputManager->zoomAdj));
	scaleMatrix[0][0] = z;
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
