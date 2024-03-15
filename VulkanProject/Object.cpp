#include "Object.h"

/*
	OBJECTS:
*/

ObjManager::Obj::Obj() {
	model = nullptr;
	orientationFlag = 0;
	color = glm::vec3(1, 1, 1);
	center = glm::vec2(0, 0);
	worldTransMatrix = glm::mat4(1);
}
ObjManager::Obj::~Obj() {

}
void ObjManager::Obj::setColor(float r, float g, float b) {
	color.r = r;
	color.g = g;
	color.b = b;
}
void ObjManager::Obj::move(XXYY adj) {

}
void ObjManager::Obj::resetCenter() {
	center = glm::vec2(0, 0);
	for (int i = 0; i < xxyys.size(); i++) {
		center += xxyys[i].getXYpos();
	}
	center.x /= float(xxyys.size());
	center.y /= float(xxyys.size());
}
void ObjManager::Obj::setPos(int anchorIndex, XXYY newPos) {
	XXYY xxyyAdj = newPos - xxyys[anchorIndex];
	center = glm::vec2(0, 0);
	for (int i = 0; i < xxyys.size(); i++) {
		xxyys[i] += xxyyAdj;
	}
	worldTransMatrix[3][0] = xxyys[0].getXYpos().x;
	worldTransMatrix[3][1] = xxyys[0].getXYpos().y;
	resetCenter();
}

void ObjManager::Obj::rotate(int anchorIndex, const int ROTATE_FLAG) {
	orientationFlag = (orientationFlag + ROTATE_FLAG) % 12;
	setRotation();

	// Rotate the xxyy points by referencing the only 12 options from one pos to another.
	XXYY storedAnchor = xxyys[anchorIndex];
	for (int i = 1; i < xxyys.size(); i++) {
		int nextIndex = (anchorIndex + 1) % xxyys.size();
		XXYY adj = xxyys[nextIndex] - storedAnchor;
		adj = getNextAdj(adj);
		storedAnchor = xxyys[nextIndex];
		xxyys[nextIndex] = xxyys[anchorIndex] + adj;
		anchorIndex = (anchorIndex + 1) % xxyys.size();
	}
	resetCenter();
}
void ObjManager::Obj::setRotation() {
	float d = getRadians(orientationFlag);
	worldTransMatrix[0][0] = cos(d);
	worldTransMatrix[0][1] = -sin(d);
	worldTransMatrix[1][0] = sin(d);
	worldTransMatrix[1][1] = cos(d);
}

// Thank god for stack overflow.
bool checkIntersect1(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
	glm::vec2 s1, s2;
	s1.x = p1.x - p0.x;
	s1.y = p1.y - p0.y;
	s2.x = p3.x - p2.x;
	s2.y = p3.y - p2.y;

	float s, t;
	s = (-s1.y * (p0.x - p2.x) + s1.x * (p0.y - p2.y)) / (-s2.x * s1.y + s1.x * s2.y);
	t = (s2.x * (p0.y - p2.y) - s2.y * (p0.x - p2.x)) / (-s2.x * s1.y + s1.x * s2.y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
		// Collision detected
		return 1;
	}

	return 0; // No collision
}

struct Point
{
	int x;
	int y;
	inline bool operator==(Point A) {
		return (A.x == x && A.y == y);
	}
};

// Given three collinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(Point p, Point q, Point r)
{
	if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
		q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
		return true;

	return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r)
{
	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/
	// for details of below formula.
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // collinear

	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool doIntersect(Point p1, Point q1, Point p2, Point q2) {
	// Find the four orientations needed for general and
	// special cases
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases
	// p1, q1 and p2 are collinear and p2 lies on segment p1q1
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and q2 are collinear and q2 lies on segment p1q1
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are collinear and p1 lies on segment p2q2
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are collinear and q1 lies on segment p2q2
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases
}

bool doIntersectContort(glm::vec2 p1, glm::vec2 q1, glm::vec2 p2, glm::vec2 q2) {
	Point pp1, pp2, qq1, qq2;
	pp1.x = int(p1.x * 1000);
	pp1.y = int(p1.y * 1000);
	pp2.x = int(p2.x * 1000);
	pp2.y = int(p2.y * 1000);
	qq1.x = int(q1.x * 1000);
	qq1.y = int(q1.y * 1000);
	qq2.x = int(q2.x * 1000);
	qq2.y = int(q2.y * 1000);

	return doIntersect(pp1, qq1, pp2, qq2);
}

void ObjManager::Obj::pushDataToVulkan(std::vector<Vertex>* outVertices, std::vector<uint16_t>* outIndices, std::vector <glm::mat4>* outTranslationMatrices) {
	int transMatrixIndex = (int)outTranslationMatrices->size();
	outTranslationMatrices->push_back(worldTransMatrix);

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

/*
	OBJECT MANAGER:
*/

void ObjManager::defineModels(std::vector<Model>& models) {
	Model triangle("models/TriangleShadow.obj");
	models.push_back(triangle);

	Model square("models/SquareShadow.obj");
	models.push_back(square);

	Model lineSegment("models/LineSegment.obj");
	models.push_back(lineSegment);

	Model point("models/Coin.obj");
	models.push_back(point);
}
void ObjManager::defineBaseObjects(std::vector<Obj>& baseObjs) {
	Obj triangle;
	triangle.xxyys.push_back(XXYY(0, 0, 0, 0));  //   /\ 
	triangle.xxyys.push_back(XXYY(2, 0, 0, -6)); //  /  \ 
	triangle.xxyys.push_back(XXYY(4, 0, 0, 0));  // /____\ 
	triangle.resetCenter();
	triangle.model = &m_models[0];
	baseObjs.push_back(triangle);

	Obj square;
	square.xxyys.push_back(XXYY(0, 0, 0, 0));  //  ______
	square.xxyys.push_back(XXYY(0, 0, -4, 0)); // |      |
	square.xxyys.push_back(XXYY(4, 0, -4, 0)); // |      |
	square.xxyys.push_back(XXYY(4, 0, 0, 0));  // |______|
	square.resetCenter();
	square.model = &m_models[1];
	baseObjs.push_back(square);

	Obj lineSegment;
	lineSegment.xxyys.push_back(XXYY(0, 0, 0, 0));
	lineSegment.xxyys.push_back(XXYY(4, 0, 0, 0));
	lineSegment.resetCenter();
	lineSegment.model = &m_models[2];
	baseObjs.push_back(lineSegment);

	Obj point;
	point.xxyys.push_back(XXYY(0, 0, 0, 0));
	point.resetCenter();
	point.model = &m_models[3];
	baseObjs.push_back(point);
}

ObjManager::ObjManager() {
	p_inputManager = nullptr;
	p_camera = nullptr;
	p_outVertices = nullptr;
	p_outIndices = nullptr;

	defineModels(m_models);
	defineBaseObjects(m_baseObjs);
	m_previewObjBaseIndex = 0;
	m_previewObj = m_baseObjs[m_previewObjBaseIndex];
	m_previewObj.setColor(1, 1, 1);

	m_objs.push_back(m_baseObjs[0]);
}
void ObjManager::init(GLFWwindow* window, InputManager* inputManager, GUIManager* guiManager, Camera* camera,
	std::vector<Vertex>* vertices, std::vector<uint16_t>* indices, std::vector <glm::mat4>* objMatrices) {
	p_window = window;
	p_inputManager = inputManager;
	p_guiManager = guiManager;
	p_camera = camera;
	p_outVertices = vertices;
	p_outIndices = indices;
	p_outTranslationMatrices = objMatrices;
}
void setPreviewObjToCursor(ObjManager::Obj& previewObj, Camera* camera) {
	//previewObj.worldTransMatrix = glm::mat4(1);
	//previewObj.resetCenter();
	glm::vec2 offset = previewObj.xxyys[0].getXYpos() - previewObj.center;

	previewObj.worldTransMatrix[3][0] = camera->cursorPlanePos.x + offset.x;
	previewObj.worldTransMatrix[3][1] = camera->cursorPlanePos.y + offset.y;
}

void ObjManager::update() {
	if (p_guiManager->layouts[LAYOUT_TEST].buttons[BUTTON_BUILDING_TYPE_0].pressed && p_inputManager->leftClick.click) {
		m_previewObj = m_baseObjs[OBJ_TYPE_TRIANGLE];
	}
	else if (p_guiManager->layouts[LAYOUT_TEST].buttons[BUTTON_BUILDING_TYPE_1].pressed && p_inputManager->leftClick.click) {
		m_previewObj = m_baseObjs[OBJ_TYPE_SQUARE];
	}
	if (p_inputManager->changePreviewObjType.click == true) {
		m_previewObjBaseIndex++;
		m_previewObjBaseIndex %= m_baseObjs.size();
		m_previewObj = m_baseObjs[m_previewObjBaseIndex];
	}
	if (p_inputManager->changePreviewBuildingOrientation.click == true) {
		m_previewObj.rotate(0, ROTATE_30);
	}

	setPreviewObjToCursor(m_previewObj, p_camera);
	if (tryToSnapToNearbyObj()) {
		if (p_inputManager->click) {
			//m_previewObj.worldTransMatrix = glm::mat4(1);
			m_previewObj.worldTransMatrix[3][2] = 0;
			m_objs.push_back(m_previewObj);

			// fun but change later:
			glm::vec3 newColor;
			newColor.r = 1.0f * rand() / RAND_MAX;
			newColor.g = 1.0f * rand() / RAND_MAX;
			newColor.b = 1.0f * rand() / RAND_MAX;
			m_previewObj.color = newColor;
		}
	}
	else {
		setPreviewObjToCursor(m_previewObj, p_camera);
	}
	m_previewObj.worldTransMatrix[3][2] = 0.1f;

	if (p_inputManager->rightClick.click) { tryToDeleteObject(); }

	pushDataToVulkan();
}

struct xxyyPair {
	XXYY* xxyy;
	int previewXXYYIndex;
	float distBetween;
	xxyyPair(XXYY* xxyy, int previewXXYYIndex, float distBetween) :
		xxyy(xxyy), previewXXYYIndex(previewXXYYIndex), distBetween(distBetween) {}
};
bool sortXXYYpairs(xxyyPair a, xxyyPair b) {
	return a.distBetween < b.distBetween;
}

bool isPointInPoly(std::vector<XXYY> xxyys, glm::vec2 testPt) {
	int i, j;
	bool c = false;
	for (i = 0, j = (int)xxyys.size() - 1; i < xxyys.size(); j = i++) {
		glm::vec2 verti = xxyys[i].getXYpos();
		glm::vec2 vertj = xxyys[j].getXYpos();
		if (((verti.y > testPt.y) != (vertj.y > testPt.y)) &&
			(testPt.x < (vertj.x - verti.x) * (testPt.y - verti.y) / (vertj.y - verti.y) + verti.x))
			c = !c;
	}
	return c;
}

// Returns in [left, right, top, bottom] format.
glm::vec4 getBoundingBox(std::vector<XXYY>xxyys, glm::vec2 center) {
	float left = center.x, right = center.x, top = center.y, bottom = center.y;
	for (XXYY& currentXXYY : xxyys) {
		glm::vec2 currentPoint = currentXXYY.getXYpos();
		left = std::min(left, currentPoint.x);
		right = std::max(right, currentPoint.x);
		top = std::max(top, currentPoint.y);
		bottom = std::min(bottom, currentPoint.y);
	}
	return glm::vec4(left, right, top, bottom);
}

bool ObjManager::Obj::BoundingBoxoverlaps(Obj& other) {
	glm::vec4 thisBox = getBoundingBox(this->xxyys, this->center);
	glm::vec4 otherBox = getBoundingBox(other.xxyys, other.center);

	if (thisBox[0] < otherBox[1] && thisBox[1] > otherBox[0] &&
		thisBox[3] < thisBox[2] && thisBox[2] > otherBox[3]) {
		return true;
	}
	return false;
}

bool envelopedOrEnveloping(ObjManager::Obj A, ObjManager::Obj B) {
	if (A.xxyys.size() > 2 && B.xxyys.size() > 2) {
		return false;
	}
	int xxyyMatches = 0;

	for (XXYY aXXYY : A.xxyys) {
		for (XXYY bXXYY : B.xxyys) {
			if (aXXYY == bXXYY) {
				xxyyMatches++;
				break;
			}
		}
	}
	if (xxyyMatches == A.xxyys.size()) {
		return true; // A is subsumed by B.
	}
	xxyyMatches = 0;
	for (XXYY bXXYY : B.xxyys) {
		for (XXYY aXXYY : A.xxyys) {
			if (aXXYY == bXXYY) {
				xxyyMatches++;
				break;
			}
		}
	}
	if (xxyyMatches == A.xxyys.size()) {
		return true; // B is subsumed by A.
	}
	return false;
}

bool ObjManager::Obj::overlaps(Obj& other) {
	if (!BoundingBoxoverlaps(other) && this->xxyys.size() > 2) {
		return false; // Quick cull:
	}

	int thisMatchCount = 0;
	int thisTotalMatchCount = 0;
	int otherMatchCount = 0;

	std::vector<int> thisMatches;
	std::vector<int> otherMatches;

	bool checkEnveloping = this->xxyys.size() > 1 && other.xxyys.size() > 1;

	for (int i = 0; i < xxyys.size(); i++) {
		otherMatchCount = 0;
		thisMatchCount = 0;

		glm::vec2 p1 = this->xxyys[i].getXYpos();
		glm::vec2 q1 = this->xxyys[(i + 1) % this->xxyys.size()].getXYpos();

		for (int j = 0; j < other.xxyys.size(); j++) {
			if (xxyys[i] == other.xxyys[j]) {
				thisMatchCount++;
				thisTotalMatchCount++;
				thisMatches.push_back(i);
				otherMatches.push_back(j);
			}
			int thisOtherMatchCount = 0;
			for (XXYY thisXXYY : xxyys) {
				if (thisXXYY == other.xxyys[j]) {
					otherMatchCount++;
					thisOtherMatchCount++;
				}
			}
			if (thisOtherMatchCount == 0 && isPointInPoly(xxyys, other.xxyys[j].getXYpos())) {
				return true; // <- Returns if other inside this.
			}
			glm::vec2 p2 = other.xxyys[j].getXYpos();
			glm::vec2 q2 = other.xxyys[(j + 1) % other.xxyys.size()].getXYpos();

			if (!(p1 == p2 || p1 == q2 || q1 == p2 || q1 == q2)) {
				if (doIntersectContort(p1, q1, p2, q2)) {
					return true; // <- Returns if there is an intersection.
				}
			}
		}
		if (thisMatchCount == 0 && isPointInPoly(other.xxyys, xxyys[i].getXYpos())) {
			return true; // <- Returns if inside other.
		}
	}
	if ((thisTotalMatchCount == xxyys.size() || otherMatchCount == other.xxyys.size()) && checkEnveloping) {
		return true; // <- Returns if enveloped/enveloping:
	}
	if (thisMatches.size() >= 2) {
		for (int i = 0; i < thisMatches.size() - 1; i++) {
			if (abs(thisMatches[i] - thisMatches[i + 1]) != 1 ||
				abs(otherMatches[i] - otherMatches[i + 1]) != 1) {

				// todo: figure out better way to check for this VVV
				if ((thisMatches[i] == xxyys.size() - 1 || thisMatches[i] == 0) && (thisMatches[i + 1] == xxyys.size() - 1 || thisMatches[i + 1] == 0) ||
					(otherMatches[i] == other.xxyys.size() - 1 || otherMatches[i] == 0) && (otherMatches[i + 1] == other.xxyys.size() - 1 || otherMatches[i + 1] == 0)) {
					continue; // Remember that the ends connect and should count as 'one step.'
				}
				return true; // <- Returns if a line segment crosses over a object.
			}
		}
	}
	return false;
}

bool ObjManager::tryToSnapPointToWorld() {
	float currentDist = FLT_MAX;
	glm::vec2 distFromCursor = glm::vec2(p_camera->cursorPlanePos.x, p_camera->cursorPlanePos.y) - m_previewObj.center;
	glm::vec2 previewXY = m_previewObj.xxyys[0].getXYpos() + distFromCursor;
	float maxDist = 0.333f;
	float smallestDist = FLT_MAX;
	XXYY closestXXYY;
	std::vector<XXYY> negatingPoints;

	for (Obj& currentObj : m_objs) {
		if (currentObj.xxyys.size() == 1) { // Ignore points.  No need to double up.
			negatingPoints.push_back(currentObj.xxyys[0]);
		}
	}
	for (Obj& currentObj : m_objs) {
		for (XXYY& objXXYY : currentObj.xxyys) {

			bool overlappingPoint = false;
			for (XXYY o : negatingPoints) {
				if (objXXYY == o) {
					overlappingPoint = true;
					break;
				}
			}
			if (overlappingPoint == true) {
				continue;
			}

			glm::vec2 objXY = objXXYY.getXYpos();
			currentDist = getDist(previewXY, objXY);

			if (currentDist < maxDist && currentDist < smallestDist) {
				smallestDist = currentDist;
				closestXXYY = objXXYY;
			}
		}
	}
	if (smallestDist <= 1) {
		m_previewObj.setPos(0, closestXXYY);
		return true;
	}
	return false;
}

bool ObjManager::tryToSnapToNearbyObj() {
	// If preview object is a point, the only thing keeping it from snapping is if there is another point in the same xxyy.
	if (m_previewObj.xxyys.size() == 1) {
		return tryToSnapPointToWorld();
	}
	// Make a vector that contains all preview xxyy -> obj xxyy pairs that are under the min distance
	std::vector<xxyyPair>pairCandidates;
	float smallestDist = FLT_MAX, currentDist = FLT_MAX, maxDist = 0.333f;
	glm::vec2 distFromCursor = glm::vec2(p_camera->cursorPlanePos.x, p_camera->cursorPlanePos.y) - m_previewObj.center;

	for (int previewXXYYIndex = 0; previewXXYYIndex < m_previewObj.xxyys.size(); previewXXYYIndex++) {
		glm::vec2 previewXY = m_previewObj.xxyys[previewXXYYIndex].getXYpos() + distFromCursor;

		for (Obj& currentObj : m_objs) {		
			for (XXYY& objXXYY : currentObj.xxyys) {
				glm::vec2 objXY = objXXYY.getXYpos();
				currentDist = getDist(previewXY, objXY);

				if (currentDist < maxDist) {
					pairCandidates.push_back(xxyyPair(&objXXYY, previewXXYYIndex, currentDist));
				}
			}
		}
	}
	if (pairCandidates.size() == 0) {
		return false;
	}
	// sort the vector by smallest distance
	std::sort(pairCandidates.begin(), pairCandidates.end(), sortXXYYpairs);

	// loop through the vector, connecting preview obj to the list obj xxyy, checking for overlaps
	bool overlap = false;
	for (int i = 0; i < pairCandidates.size(); i++) {
		m_previewObj.setPos(pairCandidates[i].previewXXYYIndex, *pairCandidates[i].xxyy);
		overlap = false;
		for (Obj& currentObj : m_objs) {
			if (m_previewObj.overlaps(currentObj)) {
				overlap = true;
				break;
			}
		}
		if (overlap == false) {
			return true;
		}
	}
	return false;
}

bool ObjManager::tryToDeleteObject() {
	// Find the closest object:
	float smallestDist = FLT_MAX;
	int closestObjIndex = -1;
	int objIndex = 0;
	for (Obj& currentObj : m_objs) {
		float currentDist = getDist(p_camera->cursorPlanePos, currentObj.center); // This wont work for larger objects :(
		if (currentDist < smallestDist) {
			smallestDist = currentDist;
			closestObjIndex = objIndex;
		}
		objIndex++;
	}
	if (closestObjIndex == -1) { return false; }
	// Lines and points:
	if (m_objs[closestObjIndex].xxyys.size() <= 2) {
		if (smallestDist < 0.5f) {
			m_objs.erase(m_objs.begin() + closestObjIndex);
			return true;
		}
	}

	// Check if the cursor is inside the closest object:
	bool inside = isPointInPoly(m_objs[closestObjIndex].xxyys, p_camera->cursorPlanePos);
	if (inside) {
		m_objs.erase(m_objs.begin() + closestObjIndex);
		return true;
	}
	return false;
}

void ObjManager::pushDataToVulkan() {
	m_previewObj.pushDataToVulkan(p_outVertices, p_outIndices, p_outTranslationMatrices);
	for (int currentObj = 0; currentObj < m_objs.size(); currentObj++) {
		m_objs[currentObj].pushDataToVulkan(p_outVertices, p_outIndices, p_outTranslationMatrices);
	}
}