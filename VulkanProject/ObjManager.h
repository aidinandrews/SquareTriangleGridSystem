#pragma once
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <set>

#include "inputManager.h"
#include "GUIManager.h"
#include "Camera.h"
#include "VectorHelperFunctions.h"
#include "CoordinateWrappers.h"
#include "Model.h"
#include "PointObject.h"
#include "LineSegmentObject.h"
#include "ShapeObject.h"

#define OBJ_TYPE_POINT			0
#define POINT_TYPE_NONE		   -1
#define POINT_TYPE_CIRCLE		0
#define POINT_TYPE_STAR			1

#define OBJ_TYPE_LINE_SEG		1
#define LINE_SEG_TYPE_NONE	   -1
#define LINE_SEG_TYPE_SIMPLE	0
#define LINE_SEG_TYPE_5_LONG	1
#define LINE_SEG_TYPE_BELT		2

#define OBJ_TYPE_SHAPE			2
#define SHAPE_TYPE_NONE		   -1
#define SHAPE_TYPE_TRIANGLE		0
#define SHAPE_TYPE_SQUARE		1

#define OBJ_DELETION_RANGE		0.25f
#define PREVIEW_OBJ_SNAP_RANGE	0.5f

struct PreviewObj {
	int objType;
	int objSubType;

	PointObj pointObj;
	LineSegObj lineSegObj;
	ShapeObj shapeObj;

	bool isGenerator = false;

	PreviewObj() {
		objType = OBJ_TYPE_POINT;
		objSubType = 0;
	}
};

struct XxyyPair {
	XXYY xxyy;
	int previewXxyyIndex;
	float distToPoint;

	XxyyPair(XXYY xxyy, int previewXxyyIndex, float distToPoint) :
		xxyy(xxyy), previewXxyyIndex(previewXxyyIndex), distToPoint(distToPoint)
	{}
	static bool sortXxyyPairs(XxyyPair a, XxyyPair b) {
		return a.distToPoint < b.distToPoint;
	}
};

#define BOUND_TYPE_LEFT		0
#define BOUND_TYPE_RIGHT	1
struct xBound {
	bool type; // Left (0) or Right (1)
	bool moved;
	float val; // x coordinate
	int index;
	ShapeObj* parent;

	static bool sortXBounds(xBound a, xBound b) {
		return a.val < b.val;
	}
};

struct CheckShape {
	ShapeObj* shape;
	bool added = false;
	int index;
	CheckShape(ShapeObj* shape, int index) : shape(shape), index(index) {}
	CheckShape(xBound x) : shape(x.parent), index(x.index) {}
};

class ObjManager2 {
public:
	GLFWwindow* p_window;
	Camera* p_camera;
	InputManager* p_inputManager;
	GUIManager* p_guiManager;

	std::vector<Vertex>* p_outVertices;
	std::vector<uint32_t>* p_outIndices;
	std::vector <glm::mat4>* p_outTranslationMatrices;

	std::vector<Model> m_pointModels;
	std::vector<Model> m_lineSegModels;
	std::vector<Model> m_shapeModels;

	std::vector<PointObj> m_pointObjLibrary;
	std::vector<LineSegObj> m_lineSegObjLibrary;
	std::vector<ShapeObj> m_shapeObjLibrary;

	std::vector<ShapeObj*> p_movedShapeObjs;

	std::multiset<xBound> m_xBounds;
	std::vector<CheckShape> m_scanline;
	std::multiset<ShapeObj> m_shapes;

	float universalTime = 0;
	float updates = 0;

public:
	float* p_gameStepPercent;
	std::vector<PointObj> m_pointObjs;
	std::vector<PointObj> m_pointObjGenerators;
	std::vector<LineSegObj> m_lineSegObjs;
	std::vector<LineSegObj> m_belts;
	std::vector<ShapeObj> m_shapeObjs;
	PreviewObj m_previewObj;

private:
	void defineModels();
	void defineObjs();

	void updatePreviewObj();
	void updatePreviewObjTypeAndSubType();

	void placePreviewPointObj();
	void placePreviewLineSegObj();
	void placePreviewShapeObj();

	void findXXYYPairCandidates(glm::vec2 point, int xxyyIndex, std::vector<XxyyPair>& xxyyPairCandidates);
	bool trySnapPreviewObjToWorld();
	bool trySnapPreviewPointObjToWorld();
	bool trySnapPreviewLineSegObjToWorld();
	bool trySnapPreviewShapeObjToWorld();
	bool tryAddPreviewObjToWorld();

	bool tryDeleteWorldObj();
	void tryDeleteLineSegObj(std::vector<LineSegObj>& lineSegObjs);

	void checkShapeObjCollisions();
	void checkShapeOverlaps();

	void tryGeneratePointObjs();

	void pushPreviewObjDataToVulkan();

public:
	ObjManager2();

	void init(GLFWwindow* window, InputManager* inputManager, GUIManager* guiManager, Camera* camera,
		std::vector<Vertex>* vertices, std::vector<uint32_t>* indices, std::vector <glm::mat4>* objMatrices,
		float* gameStepPercent);
	void update();
	void pushDataToVulkan();
};