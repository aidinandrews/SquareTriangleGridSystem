#include "ShapeObject.h"

ShapeObj::ShapeObj() {
	orientationFlag = ORIENTATION_0;
	color = glm::vec3(1, 1, 1);
	worldTransMatrix = glm::mat4(1);
	numVerts = 0;
	//xxyys = nullptr;
	//xys = nullptr;
}

ShapeObj::ShapeObj(std::vector<XXYY> verts, Model* model) : model(model) {
	orientationFlag = ORIENTATION_0;
	color = glm::vec3(1, 1, 1);
	worldTransMatrix = glm::mat4(1);

	numVerts = int(verts.size());

	xxyys = verts;
	for (XXYY xxyy : xxyys) {
		xys.push_back(xxyy.getXYpos());
	}
	resetXYs();
	boundingBox.sides[0] = xys[0].y;
	boundingBox.sides[1] = xys[0].x;
	boundingBox.sides[2] = xys[0].y;
	boundingBox.sides[3] = xys[0].x;
	resetBoundingBox();
	resetCenter();
}

void ShapeObj::setColor(float r, float g, float b) {
	color = glm::vec3(r, g, b);
}

void ShapeObj::resetXYs() {
	for (int i = 0; i < numVerts; i++) {
		xys[i] = xxyys[i].getXYpos();
	}
}

void ShapeObj::resetBoundingBox() {
	float tempTop = -FLT_MAX, tempBottom = FLT_MAX, tempRight = -FLT_MAX, tempLeft = FLT_MAX;
	for (int i = 0; i < numVerts; i++) {
		glm::vec2 xy = xys[i];
		if (xy.y > tempTop) { tempTop = xy.y; }
		if (xy.y < tempBottom) { tempBottom = xy.y; }
		if (xy.x > tempRight) { tempRight = xy.x; }
		if (xy.x < tempLeft) { tempLeft = xy.x; }
	}
	boundingBox.sides[BB_TOP] = tempTop;
	boundingBox.sides[BB_RIGHT] = tempRight;
	boundingBox.sides[BB_BOTTOM] = tempBottom;
	boundingBox.sides[BB_LEFT] = tempLeft;
}

void ShapeObj::resetCenter() {
	center = glm::vec2(0, 0);
	for (int i = 0; i < numVerts; i++) {
		center += xys[i];
	}
	center.x /= float(numVerts);
	center.y /= float(numVerts);
}
void ShapeObj::move(XXYY adj) {
	for (int i = 0; i < numVerts; i++) {
		xxyys[i] += adj;
		xys[i] = xxyys[i].getXYpos();
	}
	resetCenter();
	resetBoundingBox();
	worldTransMatrix[3][0] = xys[0].x;
	worldTransMatrix[3][1] = xys[0].y;
}
void ShapeObj::setPos(int xxyysIndex, XXYY newPos) {
	XXYY xxyyAdj = newPos - xxyys[xxyysIndex];
	center = glm::vec2(0, 0);
	for (int i = 0; i < numVerts; i++) {
		xxyys[i] += xxyyAdj;
	}
	resetXYs();
	resetCenter();
	resetBoundingBox();
	worldTransMatrix[3][0] = xys[0].x;
	worldTransMatrix[3][1] = xys[0].y;
}
void ShapeObj::rotate(int xxyysIndex, const int ROTATE_FLAG) {
	orientationFlag = (orientationFlag + ROTATE_FLAG) % 12;
	setRotation();
	// Rotate the xxyy points by referencing the only 12 options from one pos to another.
	int currentIndex = xxyysIndex;
	XXYY storedAnchor = xxyys[currentIndex];
	for (int i = 1; i < numVerts; i++) {
		int nextIndex = (currentIndex + 1) % numVerts;
		XXYY adj = xxyys[nextIndex] - storedAnchor;
		adj = getNextAdj(adj);
		storedAnchor = xxyys[nextIndex];
		xxyys[nextIndex] = xxyys[currentIndex] + adj;
		currentIndex = (currentIndex + 1) % numVerts;
	}
	resetXYs();
	resetBoundingBox();
	resetCenter();
}
void ShapeObj::setRotation() {
	float d = getRadians(orientationFlag);
	worldTransMatrix[0][0] = cos(d);
	worldTransMatrix[0][1] = -sin(d);
	worldTransMatrix[1][0] = sin(d);
	worldTransMatrix[1][1] = cos(d);
}

bool ShapeObj::boundingBoxoverlaps(ShapeObj& other) {
	return
		this->boundingBox.top() > other.boundingBox.bottom() &&
		this->boundingBox.right() > other.boundingBox.left() &&
		this->boundingBox.bottom() < other.boundingBox.top() &&
		this->boundingBox.left() < other.boundingBox.right();
}

bool ShapeObj::isPointInside(glm::vec2 testPt) {
	int i, j;
	bool c = false;
	for (i = 0, j = this->numVerts - 1; i < this->numVerts; j = i++) {
		glm::vec2 verti = this->xys[i];
		glm::vec2 vertj = this->xys[j];
		if (((verti.y > testPt.y) != (vertj.y > testPt.y)) &&
			(testPt.x < (vertj.x - verti.x) * (testPt.y - verti.y) / (vertj.y - verti.y) + verti.x))
			c = !c;
	}
	return c;
}

// Given three collinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool ShapeObj::onSegment(Point p, Point q, Point r) {
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
int ShapeObj::orientation(Point p, Point q, Point r) {
	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/
	// for details of below formula.
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // collinear

	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool ShapeObj::doIntersect(Point p1, Point q1, Point p2, Point q2) {
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

bool ShapeObj::doIntersectWrapper(glm::vec2 p1, glm::vec2 q1, glm::vec2 p2, glm::vec2 q2) {
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

int ShapeObj::overlaps(ShapeObj& other) {
	if (!boundingBoxoverlaps(other) && this->numVerts > 2) { // <- Quick cull
		return 1;
	}

	int thisMatchCount = 0;
	int thisTotalMatchCount = 0;
	int otherMatchCount = 0;

	std::vector<int> thisMatches;
	std::vector<int> otherMatches;

	bool checkEnveloping = this->numVerts > 1 && other.numVerts > 1;

	for (int i = 0; i < numVerts; i++) {
		otherMatchCount = 0;
		thisMatchCount = 0;

		glm::vec2 p1 = this->xys[i];
		glm::vec2 q1 = this->xys[(i + 1) % this->numVerts];

		for (int j = 0; j < other.numVerts; j++) {
			if (xxyys[i] == other.xxyys[j]) {
				thisMatchCount++;
				thisTotalMatchCount++;
				thisMatches.push_back(i);
				otherMatches.push_back(j);
			}
			int thisOtherMatchCount = 0;
			for (int k = 0; k < this->numVerts; k++) {
				if (this->xxyys[k] == other.xxyys[j]) {
					otherMatchCount++;
					thisOtherMatchCount++;
				}
			}
			if (thisOtherMatchCount == 0 && isPointInside(other.xys[j])) {
				return 0; // <- Returns if other inside this.
			}
			glm::vec2 p2 = other.xys[j];
			glm::vec2 q2 = other.xys[(j + 1) % other.numVerts];

			if (!(p1 == p2 || p1 == q2 || q1 == p2 || q1 == q2)) {
				if (doIntersectWrapper(p1, q1, p2, q2)) {
					return -1; // <- Returns if there is an intersection.
				}
			}
		}
		if (thisMatchCount == 0 && other.isPointInside(xys[i])) {
			return -2; // <- Returns if inside other.
		}
	}
	if ((thisTotalMatchCount == numVerts || otherMatchCount == other.numVerts) && checkEnveloping) {
		return -3; // <- Returns if enveloped/enveloping:
	}
	if (thisMatches.size() >= 2) {
		for (int i = 0; i < thisMatches.size() - 1; i++) {
			if (abs(thisMatches[i] - thisMatches[i + 1]) != 1 ||
				abs(otherMatches[i] - otherMatches[i + 1]) != 1) {

				// todo: figure out better way to check for this VVV
				if ((thisMatches[i] == numVerts - 1 || thisMatches[i] == 0) && (thisMatches[i + 1] == numVerts - 1 || thisMatches[i + 1] == 0) ||
					(otherMatches[i] == other.numVerts - 1 || otherMatches[i] == 0) && (otherMatches[i + 1] == other.numVerts - 1 || otherMatches[i + 1] == 0)) {
					continue; // Remember that the ends connect and should count as 'one step.'
				}
				return -4; // <- Returns if a line segment crosses over a object.
			}
		}
	}
	return 2;
}

bool ShapeObj::intersects(ShapeObj& other) {
	for (int i = 0; i < numVerts; i++) {
		glm::vec2 p1 = this->xys[i];
		glm::vec2 q1 = this->xys[(i + 1) % this->numVerts];

		for (int j = 0; j < other.numVerts; j++) {
			glm::vec2 p2 = other.xys[j];
			glm::vec2 q2 = other.xys[(j + 1) % other.numVerts];

			if (!(p1 == p2 || p1 == q2 || q1 == p2 || q1 == q2)) {
				if (doIntersectWrapper(p1, q1, p2, q2)) {
					return true; // <- Returns if there is an intersection.
				}
			}
		}
	}
	return false;
}

struct MatchIndex {
	int thisIndex;
	int otherIndex;
	MatchIndex(int thisIndex, int otherIndex) : thisIndex(thisIndex), otherIndex(otherIndex) {}
};

inline bool ShapeObj::checkSubsumedAndNonConcurrent(ShapeObj& other) {
	// Check if all xxyy points of this ShapeObj are subsumed by the other ShapeObj or vice versa:
	std::vector<MatchIndex> matchIndices;
	bool checkConcurrency = false;
	for (int thisXxyyIndex = 0; thisXxyyIndex < this->numVerts; thisXxyyIndex++) {
		for (int otherXxyyIndex = 0; otherXxyyIndex < other.numVerts; otherXxyyIndex++) {
			if (this->xxyys[thisXxyyIndex] == other.xxyys[otherXxyyIndex]) {
				matchIndices.push_back({ MatchIndex(thisXxyyIndex, otherXxyyIndex) });
			}
		}
	}
	if (matchIndices.size() >= other.numVerts || matchIndices.size() >= this->numVerts) {
		return true; // One ShapeObj has subsumed the other.
	}
	if (matchIndices.size() < 2) {
		return false; // There cannont be a non concurrance issue if there is only one match.
	}
	// Check that if the two ShapeObjs share more than one xxyy position, the xxyy indices of both sets are concurrent:
	// If the sets are not concurrent (because ShapeObjs are convex), there is an intersection that the intersection check will not find!
	for (int i = 0; i < matchIndices.size() - 1; i++) {
		int difBetweenThisIndices = abs(matchIndices[i].thisIndex - matchIndices[i + 1].thisIndex);
		if (difBetweenThisIndices != 1 && difBetweenThisIndices != this->numVerts - 1) {
			return true;
		}
		int difBetweenOtherIndices = abs(matchIndices[i].otherIndex - matchIndices[i + 1].otherIndex);
		if (difBetweenOtherIndices != 1 && difBetweenOtherIndices != this->numVerts - 1) {
			return true;
		}
	}
	return false;
}

bool ShapeObj::arePointsInside(ShapeObj& other) {
	// Check if any xxyy point in this ShapeObj is inside the other ShapeObj:
	for (int i = 0; i < this->numVerts; i++) {
		if (other.isPointInside(this->xys[i])) {
			return true;
		}
	}
	// Check if any xxyy point in the other ShapeObj is inside this ShapeObj:
	for (int i = 0; i < other.numVerts; i++) {
		if (this->isPointInside(other.xys[i])) {
			return true;
		}
	}
	return false;
}

inline bool ShapeObj::checkIntersections(ShapeObj& other) {
	// Check if there are any intersections of line segments between this and the other ShapeObj:
	for (int i = 0; i < numVerts; i++) {
		glm::vec2 p1 = this->xys[i];
		glm::vec2 q1 = this->xys[(i + 1) % this->numVerts];

		for (int j = 0; j < other.numVerts; j++) {
			glm::vec2 p2 = other.xys[j];
			glm::vec2 q2 = other.xys[(j + 1) % other.numVerts];

			if (p1 == p2 || p1 == q2 || q1 == p2 || q1 == q2) { // Intersection doesnt count if the points are the same!
				continue;
			}
			if (doIntersectWrapper(p1, q1, p2, q2)) {
				return true;
			}
		}
	}
	return false;
}

bool ShapeObj::overlaps2(ShapeObj& other) {
	// Quick cull with bounding boxes:
	if (this->numVerts > 2) {
		if (!boundingBoxoverlaps(other)) { return false; }
	}
	// Check if all xxyy points of this ShapeObj are subsumed by the other ShapeObj or vice versa:
	if (checkSubsumedAndNonConcurrent(other)) {
		std::cout << "subsumed\n";
		return true;
	}
	// Checks if any xxyy point in this ShapeObj is inside the other ShapeObj or vice versa:
	if (arePointsInside(other)) {
		std::cout << "points inside\n";
		return true;
	}
	// Checks if there are any intersections of line segments between this and the other ShapeObj:
	if (checkIntersections(other)) {
		std::cout << "intersection\n";
		return true;
	}
	// If all checks fail, there is no overlap:
	return false;
}

void ShapeObj::pushDataToVulkan(Camera& camera, std::vector<Vertex>* outVertices, std::vector<uint32_t>* outIndices, std::vector<glm::mat4>* outTranslationMatrices) {
	int transMatrixIndex = (int)outTranslationMatrices->size();
	outTranslationMatrices->push_back(worldTransMatrix);

	int indexOffset = (int)outVertices->size();
	for (int i = 0; i < model->indices.size(); i++) {
		outIndices->push_back(model->indices[i] + indexOffset);
	}
	float colorStepVal = 1.0f / model->verts.size();
	glm::vec3 colorStep(colorStepVal, colorStepVal, 0);
	for (int v = 0; v < model->verts.size(); v++) {
		Vertex temp = model->verts[v];
		temp.color = color;
		temp.modelTranformMatrixIndex = transMatrixIndex;
		outVertices->push_back(temp);
		indexOffset++;
	}
}
