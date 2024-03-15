#pragma once
#include <iostream>

#include "CoordinateWrappers.h"
#include "Model.h"
#include "Camera.h"

struct Point {
	int x;
	int y;
	inline bool operator==(Point A) {
		return (A.x == x && A.y == y);
	}
};

#define BB_TOP		0
#define BB_RIGHT	1
#define BB_BOTTOM	2
#define BB_LEFT		3

struct BoundingBox {
	float sides[4];
	BoundingBox() {
		sides[0] = 0;
		sides[1] = 0;
		sides[2] = 0;
		sides[3] = 0;
	}
	BoundingBox(float top, float right, float bottom, float left) {
		sides[0] = top;
		sides[1] = right;
		sides[2] = bottom;
		sides[3] = left;
	}
	float top()		{ return sides[0]; }
	float right()	{ return sides[1]; }
	float bottom()	{ return sides[2]; }
	float left()	{ return sides[3]; }

	void print() {
		std::cout 
			<< "top: " << sides[0] 
			<< ", right: " << sides[1]
			<< ", bottom: " << sides[2] 
			<< ", left: " << sides[3] 
			<< std::endl;
	}
};

struct ShapeObj {
	Model* model; // not used for now but after xxyy check, push these verts to vulkan!
	int orientationFlag;
	int numVerts;
	glm::vec3 color;
	std::vector<XXYY> xxyys;
	std::vector <glm::vec2> xys;
	glm::vec2 center;
	BoundingBox boundingBox;
	glm::mat4 worldTransMatrix;
	bool moved = false;

	ShapeObj();
	ShapeObj(std::vector<XXYY> verts, Model* model);
	~ShapeObj() {
		//delete[] xxyys;
		//delete[] xys;
	}

	void resetXYs();
	void resetBoundingBox();
	void resetCenter();
	
	void setColor(float r, float g, float b);
	void setPos(int anchorIndex, XXYY newPos);
	void move(XXYY adj);
	void setRotation();
	void rotate(int anchorIndex, const int ROTATE_FLAG);

	// Returns 0 if there is a connection and there is no overlap,
	// 1 if there is a connection and there is overlap,
	// and 2 if there is no connection and no overlap.
	bool boundingBoxoverlaps(ShapeObj& other);

	int overlaps(ShapeObj& other);
	bool intersects(ShapeObj& other);

	inline bool checkSubsumedAndNonConcurrent(ShapeObj& other);

	bool isPointInside(glm::vec2 testPt);
	bool arePointsInside(ShapeObj& other);

	bool onSegment(Point p, Point q, Point r);
	int orientation(Point p, Point q, Point r);

	bool doIntersect(Point p1, Point q1, Point p2, Point q2);
	bool doIntersectWrapper(glm::vec2 p1, glm::vec2 q1, glm::vec2 p2, glm::vec2 q2);
	inline bool checkIntersections(ShapeObj& other);

	bool overlaps2(ShapeObj& other);

	void pushDataToVulkan(Camera& camera, std::vector<Vertex>* outVertices, std::vector<uint32_t>* outIndices, std::vector <glm::mat4>* outTranslationMatrices);

	/*ShapeObj& operator=(const ShapeObj& other) {
		model = other.model;
		orientationFlag = other.orientationFlag;
		numVerts = other.numVerts;
		color = other.color;
		center = other.center;
		boundingBox = other.boundingBox;
		worldTransMatrix = other.worldTransMatrix;

		delete[] xxyys;
		xxyys = new XXYY[numVerts];
		for (int i = 0; i < numVerts; i++) {
			xxyys[i] = other.xxyys[i];
		}
		delete[] xys;
		xys = new glm::vec2[numVerts];
		for (int i = 0; i < numVerts; i++) {
			xys[i] = other.xys[i];
		}
		return *this;
	}
	ShapeObj(const ShapeObj& other)
	{
		operator=(other);
	}*/
};