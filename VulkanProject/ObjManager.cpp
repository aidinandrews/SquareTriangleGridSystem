#include "ObjManager.h"

void ObjManager2::defineModels() {
	// Point based models:
	Model circleNode("models/CircleNode.obj");
	m_pointModels.push_back(circleNode);
	m_pointModels.push_back(Model("models/StarNode.obj"));

	// Line segment based models:
	m_lineSegModels.push_back(Model("models/LineSegment.obj"));
	m_lineSegModels.push_back(Model("models/LineSegment4Long.obj"));
	for (Vertex& v : m_lineSegModels[1].verts) {
		v.pos.y /= 4;
	}
	m_lineSegModels.push_back(Model("models/LineSegment4Long.obj"));

	// Shape based models:
	m_shapeModels.push_back(Model("models/TriangularPyramid4Wide.obj"));
	for (Vertex& v : m_shapeModels[0].verts) {
		v.pos.z /= 4;
	}
	m_shapeModels.push_back(Model("models/SquarePyramid4Wide.obj"));
	for (Vertex& v : m_shapeModels[1].verts) {
		v.pos.z /= 4;
	}
}

void ObjManager2::defineObjs() {
	srand(unsigned int(std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1)));

	PointObj simplePointObj;
	simplePointObj.model = &m_pointModels[POINT_TYPE_CIRCLE];
	simplePointObj.color = randColor();
	m_pointObjLibrary.push_back(simplePointObj);

	PointObj starNode;
	starNode.model = &m_pointModels[POINT_TYPE_STAR];
	starNode.color = randColor();
	m_pointObjLibrary.push_back(starNode);

	LineSegObj simpleLineSegObj;
	simpleLineSegObj.model = &m_lineSegModels[LINE_SEG_TYPE_SIMPLE];
	simpleLineSegObj.color = randColor();
	m_lineSegObjLibrary.push_back(simpleLineSegObj);

	LineSegObj lineSegObj4Long;
	lineSegObj4Long.model = &m_lineSegModels[LINE_SEG_TYPE_5_LONG];
	lineSegObj4Long.xxyys[0] = XXYY(0, 0, 0, 0);
	lineSegObj4Long.xxyys[1] = XXYY(16, 0, 0, 0);
	lineSegObj4Long.updateSpine();
	lineSegObj4Long.color = randColor();
	m_lineSegObjLibrary.push_back(lineSegObj4Long);

	LineSegObj belt = lineSegObj4Long;
	belt.model = &m_lineSegModels[LINE_SEG_TYPE_BELT];
	belt.color = randColor();
	m_lineSegObjLibrary.push_back(belt);

	std::vector<XXYY> triangleXXYYs;
	triangleXXYYs.push_back(XXYY(0, 0, 0, 0));
	triangleXXYYs.push_back(XXYY(8, 0, 0, -24));
	triangleXXYYs.push_back(XXYY(16, 0, 0, 0));
	m_shapeObjLibrary.push_back(ShapeObj(triangleXXYYs, &m_shapeModels[SHAPE_TYPE_TRIANGLE]));

	std::vector<XXYY> squareXXYYs;
	squareXXYYs.push_back(XXYY(0, 0, 0, 0));
	squareXXYYs.push_back(XXYY(0, 0, -16, 0));
	squareXXYYs.push_back(XXYY(16, 0, -16, 0));
	squareXXYYs.push_back(XXYY(16, 0, 0, 0));
	m_shapeObjLibrary.push_back(ShapeObj(squareXXYYs, &m_shapeModels[SHAPE_TYPE_SQUARE]));
}

bool operator<(xBound a, xBound b) {
	return a.val < b.val;
}

ObjManager2::ObjManager2() {
	p_inputManager = nullptr;
	p_camera = nullptr;
	p_outVertices = nullptr;
	p_outIndices = nullptr;

	defineModels();
	defineObjs();

	m_previewObj.pointObj = m_pointObjLibrary[POINT_TYPE_CIRCLE];
	m_previewObj.lineSegObj = m_lineSegObjLibrary[LINE_SEG_TYPE_SIMPLE];
	m_previewObj.shapeObj = m_shapeObjLibrary[SHAPE_TYPE_TRIANGLE];

	m_pointObjs.push_back(m_pointObjLibrary[POINT_TYPE_CIRCLE]);


	int width = 3000, height = 10;
	for (int i = 0; i < height; i++) {
		ShapeObj s = m_shapeObjLibrary[SHAPE_TYPE_TRIANGLE];
		s.move(XXYY(0, 0, 16, 0) * i);
		for (int j = 0; j < width; j++) {
			if (j % 3 == 0) {
				s.color = glm::vec3(1, 1, 0);
				s.moved = true;
				s.move(XXYY(16, 0, 0, 0));
			}
			else {
				s.color = glm::vec3(1, 1, 1);
				s.moved = false;
				s.move(XXYY(16, 0, 0, 0));
			}
			m_shapeObjs.push_back(s);

			xBound xb;
			xb.type = BOUND_TYPE_LEFT;
			xb.val = s.boundingBox.left();
			xb.parent = &m_shapeObjs[m_shapeObjs.size() - 1];
			xb.moved = s.moved;
			xb.index = i * width + j;
			m_xBounds.insert(xb);
			xb.type = BOUND_TYPE_RIGHT;
			xb.val = s.boundingBox.right();
			m_xBounds.insert(xb);
		}
	}
}

void ObjManager2::init(GLFWwindow* window, InputManager* inputManager, GUIManager* guiManager, Camera* camera,
	std::vector<Vertex>* vertices, std::vector<uint32_t>* indices, std::vector <glm::mat4>* objMatrices,
	float* gameStepPercent) {
	p_window = window;
	p_inputManager = inputManager;
	p_guiManager = guiManager;
	p_camera = camera;
	p_outVertices = vertices;
	p_outIndices = indices;
	p_outTranslationMatrices = objMatrices;
	p_gameStepPercent = gameStepPercent;
}

void ObjManager2::placePreviewPointObj() {
	m_previewObj.pointObj.xy = p_camera->cursorPlanePos;
	m_previewObj.pointObj.worldTransMatrix[3][0] = p_camera->cursorPlanePos.x;
	m_previewObj.pointObj.worldTransMatrix[3][1] = p_camera->cursorPlanePos.y;
}

void ObjManager2::placePreviewLineSegObj() {
	glm::vec2 offset = m_previewObj.lineSegObj.spine[0] - m_previewObj.lineSegObj.spine[2];
	m_previewObj.lineSegObj.worldTransMatrix[3][0] = p_camera->cursorPlanePos.x + offset.x;
	m_previewObj.lineSegObj.worldTransMatrix[3][1] = p_camera->cursorPlanePos.y + offset.y;
}

void ObjManager2::placePreviewShapeObj() {
	glm::vec2 offset = m_previewObj.shapeObj.xxyys[0].getXYpos() - m_previewObj.shapeObj.center;
	/*m_previewObj.shapeObj.resetXYs();
	for (glm::vec2& xy : m_previewObj.shapeObj.xys) { xy += (glm::vec2(p_camera->cursorPlanePos) + offset); }
	std::cout << m_previewObj.shapeObj.xys[0].x << std::endl;
	m_previewObj.shapeObj.resetBoundingBox();
	m_previewObj.shapeObj.resetCenter();*/

	m_previewObj.shapeObj.worldTransMatrix[3][0] = p_camera->cursorPlanePos.x + offset.x;
	m_previewObj.shapeObj.worldTransMatrix[3][1] = p_camera->cursorPlanePos.y + offset.y;
}

void ObjManager2::findXXYYPairCandidates(glm::vec2 point, int pointIndex, std::vector<XxyyPair>& xxyyPairCandidates) {
	for (PointObj& pointObj : m_pointObjs) { // Check all points.
		float dist = getDist(point, pointObj.xy);
		if (dist <= PREVIEW_OBJ_SNAP_RANGE) {
			xxyyPairCandidates.push_back(XxyyPair(pointObj.xxyy, pointIndex, dist));
		}
	}
	for (LineSegObj& lineSegObj : m_lineSegObjs) { // Check all line segments.
		float dist = getDist(point, lineSegObj.spine[0]);

		if (dist <= PREVIEW_OBJ_SNAP_RANGE) {
			xxyyPairCandidates.push_back(XxyyPair(lineSegObj.xxyys[0], pointIndex, dist));
		}
		dist = getDist(point, lineSegObj.spine[4]);
		if (dist <= PREVIEW_OBJ_SNAP_RANGE) {
			xxyyPairCandidates.push_back(XxyyPair(lineSegObj.xxyys[1], pointIndex, dist));
		}
	}
	for (LineSegObj& belt : m_belts) { // Check all line segments.
		XXYY xxyyAdjAdj = (belt.xxyys[1] - belt.xxyys[0]) / 4;
		XXYY xxyyAdj = XXYY(0, 0, 0, 0);
		for (glm::vec2& bone : belt.spine) {
			float dist = getDist(point, bone);
			if (dist <= PREVIEW_OBJ_SNAP_RANGE) {
				xxyyPairCandidates.push_back(XxyyPair(belt.xxyys[0] + xxyyAdj, pointIndex, dist));
			}
			xxyyAdj += xxyyAdjAdj;
		}
	}
	for (ShapeObj& shapeObj : m_shapeObjs) { // Check all shapes.
		for (int i = 0; i < shapeObj.numVerts; i++) {
			float dist = getDist(point, shapeObj.xys[i]);
			if (dist <= PREVIEW_OBJ_SNAP_RANGE) {
				xxyyPairCandidates.push_back(XxyyPair(shapeObj.xxyys[i], pointIndex, dist));
			}
		}
	}
}

bool ObjManager2::trySnapPreviewPointObjToWorld() {
	std::vector<XxyyPair> xxyyPairCandidates;
	findXXYYPairCandidates(m_previewObj.pointObj.xy, 0, xxyyPairCandidates);
	std::sort(xxyyPairCandidates.begin(), xxyyPairCandidates.end(), XxyyPair::sortXxyyPairs);

	if (xxyyPairCandidates.size() > 0) {
		m_previewObj.pointObj.setPos(xxyyPairCandidates[0].xxyy);
		return true;
	}
	return false;
}

bool ObjManager2::trySnapPreviewLineSegObjToWorld() {
	std::vector<XxyyPair> xxyyPairCandidates;
	glm::vec2 p1 = m_previewObj.lineSegObj.spine[0];
	glm::vec2 p2 = m_previewObj.lineSegObj.spine[4];
	glm::vec2 adj = glm::vec2(p_camera->cursorPlanePos) - m_previewObj.lineSegObj.spine[2];
	findXXYYPairCandidates(p1 + adj, 0, xxyyPairCandidates);
	findXXYYPairCandidates(p2 + adj, 1, xxyyPairCandidates);

	std::sort(xxyyPairCandidates.begin(), xxyyPairCandidates.end(), XxyyPair::sortXxyyPairs);

	if (xxyyPairCandidates.size() > 0) {
		m_previewObj.lineSegObj.setPos(xxyyPairCandidates[0].previewXxyyIndex, xxyyPairCandidates[0].xxyy);
		return true;
	}
	return false;
}

bool ObjManager2::trySnapPreviewShapeObjToWorld() {
	std::vector<XxyyPair> xxyyPairCandidates;
	glm::vec2 adj = glm::vec2(p_camera->cursorPlanePos) - m_previewObj.shapeObj.center;

	for (int i = 0; i < m_previewObj.shapeObj.numVerts; i++) {
		glm::vec2 pt = m_previewObj.shapeObj.xys[i];
		findXXYYPairCandidates(pt + adj, i, xxyyPairCandidates);
	}
	std::sort(xxyyPairCandidates.begin(), xxyyPairCandidates.end(), XxyyPair::sortXxyyPairs);

	if (xxyyPairCandidates.size() == 0) { return false; }

	for (XxyyPair& xxyyPair : xxyyPairCandidates) {
		m_previewObj.shapeObj.setPos(xxyyPair.previewXxyyIndex, xxyyPair.xxyy);
		bool isOverlapping = false;
		for (ShapeObj& shapeObj : m_shapeObjs) {
			int debugCheck = m_previewObj.shapeObj.overlaps(shapeObj);
			if (debugCheck <= 0) {
				isOverlapping = true;
				break;
			}
		}

		if (!isOverlapping) { return true; }
	}
	return false;
}

void ObjManager2::tryDeleteLineSegObj(std::vector<LineSegObj>& lineSegObjs) {
	for (int i = 0; i < lineSegObjs.size(); i++) {
		for (glm::vec2& bone : lineSegObjs[i].spine) {
			if (getDist(bone, p_camera->cursorPlanePos) < OBJ_DELETION_RANGE) {
				lineSegObjs.erase(lineSegObjs.begin() + i);
			}
		}
	}
}

bool ObjManager2::tryDeleteWorldObj() {
	if (!p_inputManager->rightClick.click) {
		return false;
	}
	for (int i = 0; i < m_pointObjs.size(); i++) {
		if (getDist(m_pointObjs[i].xy, p_camera->cursorPlanePos) < OBJ_DELETION_RANGE) {
			m_pointObjs.erase(m_pointObjs.begin() + i);
			return true;
		}
	}
	tryDeleteLineSegObj(m_lineSegObjs);
	tryDeleteLineSegObj(m_belts);

	for (int i = 0; i < m_shapeObjs.size(); i++) {
		if (m_shapeObjs[i].isPointInside(glm::vec2(p_camera->cursorPlanePos))) {
			m_shapeObjs.erase(m_shapeObjs.begin() + i);
			return true;
		}
	}
	return false;
}

void ObjManager2::updatePreviewObjTypeAndSubType() {
	if (p_inputManager->changePreviewObjType.click) { // <- todo: make this not ass
		m_previewObj.objType++;
		m_previewObj.objType %= 3;
		m_previewObj.objSubType = 0;
	}
	if (p_inputManager->changePreviewObjSubType.click) {
		m_previewObj.objSubType++;
		switch (m_previewObj.objType) {
		case OBJ_TYPE_POINT:
			m_previewObj.objSubType %= m_pointObjLibrary.size();
			m_previewObj.pointObj = m_pointObjLibrary[m_previewObj.objSubType];
			break;
		case OBJ_TYPE_LINE_SEG:
			m_previewObj.objSubType %= m_lineSegObjLibrary.size();
			m_previewObj.lineSegObj = m_lineSegObjLibrary[m_previewObj.objSubType];
			break;
		case OBJ_TYPE_SHAPE:
			m_previewObj.objSubType %= m_shapeObjLibrary.size();
			m_previewObj.shapeObj = m_shapeObjLibrary[m_previewObj.objSubType];
			break;
		}
	}
}

bool ObjManager2::trySnapPreviewObjToWorld() {
	bool snapped = false;
	switch (m_previewObj.objType) {
	case OBJ_TYPE_POINT:
		if (p_inputManager->changePreviewBuildingOrientation.click) {
			m_previewObj.pointObj.rotate(ROTATE_30);
		}
		placePreviewPointObj();
		snapped = trySnapPreviewPointObjToWorld();
		if (!snapped) { placePreviewPointObj(); }
		return snapped;
	case OBJ_TYPE_LINE_SEG:
		if (p_inputManager->changePreviewBuildingOrientation.click) {
			m_previewObj.lineSegObj.rotate(0, ROTATE_30);
		}
		placePreviewLineSegObj();
		snapped = trySnapPreviewLineSegObjToWorld();
		if (!snapped) { placePreviewLineSegObj(); }
		return snapped;
	case OBJ_TYPE_SHAPE:
		if (p_inputManager->changePreviewBuildingOrientation.click) {
			m_previewObj.shapeObj.rotate(0, ROTATE_30);
		}
		placePreviewShapeObj();
		snapped = trySnapPreviewShapeObjToWorld();
		if (!snapped) { placePreviewShapeObj(); }
		return snapped;
	}
	return false;
}

bool ObjManager2::tryAddPreviewObjToWorld() {
	if (!p_inputManager->leftClick.click) {
		return false;
	}
	switch (m_previewObj.objType) {
	case OBJ_TYPE_POINT:
		if (m_previewObj.objSubType == POINT_TYPE_STAR) {
			m_pointObjGenerators.push_back(m_previewObj.pointObj);
			return true;
		}
		else {
			m_pointObjs.push_back(m_previewObj.pointObj);
			return true;
		}
	case OBJ_TYPE_LINE_SEG:
		if (m_previewObj.objSubType == LINE_SEG_TYPE_BELT) {
			m_belts.push_back(m_previewObj.lineSegObj);
			return true;
		}
		m_lineSegObjs.push_back(m_previewObj.lineSegObj);
		return true;
	case OBJ_TYPE_SHAPE:
		//m_previewObj.shapeObj.resetXYs();
		//m_previewObj.shapeObj.resetBoundingBox();
		m_shapeObjs.push_back(m_previewObj.shapeObj);
		return true;
	}
	return false;
}

void ObjManager2::updatePreviewObj() {
	updatePreviewObjTypeAndSubType();
	bool snapped = trySnapPreviewObjToWorld();
	if (snapped) { tryAddPreviewObjToWorld(); }
	tryDeleteWorldObj();
}

void checkForIntersections(CheckShape& shape, std::vector<CheckShape>& shapeList, std::vector<ShapeObj>& shapeObjs, std::vector<ShapeObj*>& shapeDeleteList) {
	bool overlap = false;
	int intersectChecks = 0;
	for (int i = 0; i < shapeList.size(); i++) {
		if (shapeObjs[shapeList[i].index].boundingBox.top() <= shapeObjs[shape.index].boundingBox.bottom() ||
			shapeObjs[shapeList[i].index].boundingBox.bottom() >= shapeObjs[shape.index].boundingBox.top()) {
			continue;
		}
		bool overlapCheck = shapeObjs[shapeList[i].index].intersects(shapeObjs[shape.index]);
		if (overlapCheck) {
			intersectChecks++;
			overlap = true;
			if (shapeList[i].added == false) {
				shapeObjs[shapeList[i].index].color = glm::vec3(1, 0, 0);
				shapeList[i].added = true;
				shapeDeleteList.push_back(shapeList[i].shape);
			}
		}
	}
	if (overlap) {
		shapeObjs[shape.index].color = glm::vec3(1, 0, 0);
		shape.added = true;
		shapeDeleteList.push_back(shape.shape);
	}
}

void checkForIntersections(CheckShape& shape, std::set<CheckShape>& shapeSet, std::set<int>& shapeIndexDeleteList) {
	bool overlap = false;
	for (auto& itr : shapeSet) {
		if (itr.shape->boundingBox.top() <= shape.shape->boundingBox.bottom() ||
			itr.shape->boundingBox.bottom() >= shape.shape->boundingBox.top()) {
			continue;
		}
		int overlapCheck = itr.shape->intersects(*shape.shape);
		if (overlapCheck <= 0) {
			overlap = true;
			shapeIndexDeleteList.insert(itr.index);
		}
	}
	if (overlap) {
		shape.added = true;
		shapeIndexDeleteList.insert(shape.index);
	}
}

void createXBoundList(std::vector<ShapeObj>& shapes, std::vector<xBound>& xBounds) {
	xBound temp;
	xBounds.reserve(shapes.size() * 2);
	for (int i = 0; i < shapes.size(); i++) {
		temp.type = BOUND_TYPE_LEFT;
		temp.val = shapes[i].boundingBox.left();
		temp.parent = &shapes[i];
		temp.moved = shapes[i].moved;
		temp.index = i;
		xBounds.push_back(temp);
		temp.type = BOUND_TYPE_RIGHT;
		temp.val = shapes[i].boundingBox.right();
		xBounds.push_back(temp);
	}
	std::sort(xBounds.begin(), xBounds.end(), xBound::sortXBounds);
}

void findAndDeleteXBound(xBound bound, std::vector<CheckShape>& list) {
	for (int i = 0; i < list.size(); i++) {
		if (list[i].shape == bound.parent) {
			list.erase(list.begin() + i);
			return;
		}
	}
}

struct compareShapeObjs {
	bool operator()(const ShapeObj* lhs, const ShapeObj* rhs) {
		return lhs < rhs;
	}
};

void ObjManager2::checkShapeObjCollisions() {
	//std::vector<CheckShape> checkShapesStatic;
	//std::vector<CheckShape> checkShapesMoved;
	std::vector<ShapeObj*> shapeDeleteList;
	//std::set<CheckShape> staticShapeSweepList;
	//std::set<CheckShape> dynamicShapeSweepList;
	std::set<int> shapeIndexDeleteList;

	auto startFrame = std::chrono::high_resolution_clock::now();

	std::multiset<xBound>::iterator itr = m_xBounds.begin();
	CheckShape newShape(*itr);
	for (; itr != m_xBounds.end(); ++itr) {
		if (itr->type == BOUND_TYPE_RIGHT) {
			findAndDeleteXBound(*itr, m_scanline);
			continue;
		}
		else { // itr->type == BOUND_TYPE_LEFT
			newShape.shape = itr->parent;
			newShape.index = itr->index;
			checkForIntersections(newShape, m_scanline, m_shapeObjs, shapeDeleteList);
			m_scanline.push_back(CheckShape(*itr));
		}
	}
	auto endFrame = std::chrono::high_resolution_clock::now();
	float frameTime = std::chrono::duration<float, std::chrono::milliseconds::period>(endFrame - startFrame).count();
	//std::cout << "intersect time:" << frameTime << " fps: " << 1 / (frameTime / 1000) << std::endl;
	universalTime += frameTime;

	// Delete the ovelapping shapes:
	/*for (ShapeObj*& p : shapeDeleteList) {

	}*/
}

void ObjManager2::checkShapeOverlaps() {

}


void ObjManager2::update() {
	//auto startFrame = std::chrono::high_resolution_clock::now();
	updates++;
	updatePreviewObj();
	checkShapeObjCollisions();
	//std::cout << "average update time: " << universalTime / updates << " fps: " << 1 / ((universalTime / updates) / 1000) << "\n\n";
	if (updates > 1000) {
		updates = 0;
		universalTime = 0;
	}
	//auto endFrame = std::chrono::high_resolution_clock::now();
	//float frameTime = std::chrono::duration<float, std::chrono::milliseconds::period>(endFrame - startFrame).count();
	//std::cout << "overlap frame time:" << frameTime << " fps: " << 1 / (frameTime / 1000) << std::endl;

	//startFrame = std::chrono::high_resolution_clock::now();

	pushDataToVulkan();

	//endFrame = std::chrono::high_resolution_clock::now();
	//frameTime = std::chrono::duration<float, std::chrono::milliseconds::period>(endFrame - startFrame).count();
	//std::cout << "push frame time:" << frameTime << " fps: " << 1 / (frameTime / 1000) << std::endl << std::endl;
}

void ObjManager2::pushDataToVulkan() {
	pushPreviewObjDataToVulkan();

	for (PointObj& pointObj : m_pointObjs) {
		pointObj.pushDataToVulkan(*p_camera, p_outVertices, p_outIndices, p_outTranslationMatrices);
	}
	for (LineSegObj& lineSegObj : m_lineSegObjs) {
		lineSegObj.pushDataToVulkan(*p_camera, p_outVertices, p_outIndices, p_outTranslationMatrices);
	}
	for (LineSegObj& belt : m_belts) {
		belt.pushBeltDataToVulkan(*p_camera, p_outVertices, p_outIndices, p_outTranslationMatrices, p_gameStepPercent);
	}
	for (ShapeObj& shapeObj : m_shapeObjs) {
		shapeObj.pushDataToVulkan(*p_camera, p_outVertices, p_outIndices, p_outTranslationMatrices);
	}
}

void ObjManager2::pushPreviewObjDataToVulkan() {
	switch (m_previewObj.objType) {
	case OBJ_TYPE_POINT:
		m_previewObj.pointObj.pushDataToVulkan(*p_camera, p_outVertices, p_outIndices, p_outTranslationMatrices);
		break;
	case OBJ_TYPE_LINE_SEG:
		if (m_previewObj.objSubType == LINE_SEG_TYPE_BELT) {
			m_previewObj.lineSegObj.pushBeltDataToVulkan(*p_camera, p_outVertices, p_outIndices, p_outTranslationMatrices, p_gameStepPercent);
			break;
		}
		m_previewObj.lineSegObj.pushDataToVulkan(*p_camera, p_outVertices, p_outIndices, p_outTranslationMatrices);
		break;
	case OBJ_TYPE_SHAPE:
		m_previewObj.shapeObj.pushDataToVulkan(*p_camera, p_outVertices, p_outIndices, p_outTranslationMatrices);
		break;
	}
}