#include<iostream>
#include <array>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <set>
#include <chrono>
#include <time.h>

#include "ShapeObject.h"
#include "ObjManager.h"

extern std::chrono::steady_clock::time_point GLOBAL_START_TIME;

inline void startTimer() {
	GLOBAL_START_TIME = std::chrono::high_resolution_clock::now();
}
inline float endTimer(std::string identifier, bool print) {
	auto end = std::chrono::high_resolution_clock::now();
	float frameTime = std::chrono::duration<float, std::chrono::milliseconds::period>(end - GLOBAL_START_TIME).count();
	if (print)
		std::cout << identifier << ": " << frameTime << "ms, fps: " << 1 / (frameTime / 1000) << std::endl;
	return frameTime;
}

#define BOUND_TYPE_LEFT		0
#define BOUND_TYPE_RIGHT	1
struct XBound {
	bool type; // Left (0) or Right (1)
	bool moved;
	float val; // x coordinate
	int index;
	ShapeObj* parent;

	static bool sortXBounds(XBound a, XBound b) {
		return a.val < b.val;
	}
};

//struct BB {
//	glm::vec4 coords;
//	ShapeObj* parent;
//
//	void setMin(float x, float y) {
//		coords[0] = x;
//		coords[1] = y;
//	}
//	void setMax(float x, float y) {
//		coords[2] = x;
//		coords[3] = y;
//	}
//	inline float getTop() {
//		return coords[3];
//	}
//	inline float getBottom() {
//		return coords[1];
//	}
//	inline float getLeft() {
//		return coords[0];
//	}
//	inline float getRight() {
//		return coords[2];
//	}
//};
//
//void setTest() {
//	srand(time(NULL));
//	std::set<int> S;
//	const int SIZE = 100000;
//
//	for (int i = 0; i < SIZE; i++) {
//		int r = int(((1.0f * rand()) / RAND_MAX) * float(SIZE));
//		S.insert(i);
//	}
//
//	std::set<int>::iterator itr;
//
//	const int LOOPS = 1000;
//	float totalTime = 0;
//	for (int i = 0; i < LOOPS; i++) {
//		startTimer();
//		for (itr = S.begin(); itr != S.end(); itr++) {
//			int a = *itr;
//		}
//		totalTime += endTimer("set iterate", false);
//	}
//	std::cout << "set iterate: " << totalTime / LOOPS << "ms, fps: " << 1 / (totalTime / LOOPS / 1000) << std::endl;
//
//	totalTime = 0;
//	for (int i = 0; i < LOOPS; i++) {
//		startTimer();
//		// Displaying set elements
//		for (int i = 0; i < 1000; i++) {
//			S.insert(-i);
//		}
//		totalTime += endTimer("set insert", false);
//	}
//	std::cout << "set insert: " << totalTime / LOOPS << "ms, fps: " << 1 / (totalTime / LOOPS / 1000) << std::endl;
//}
//
//void multiSetTest() {
//	srand(time(NULL));
//	std::multiset<int> S;
//	const int SIZE = 100000;
//
//	for (int i = 0; i < SIZE; i++) {
//		int r = int(((1.0f * rand()) / RAND_MAX) * float(SIZE));
//		S.insert(i);
//	}
//
//	std::set<int>::iterator itr;
//
//	const int LOOPS = 1000;
//	float totalTime = 0;
//	for (int i = 0; i < LOOPS; i++) {
//		startTimer();
//		for (itr = S.begin(); itr != S.end(); itr++) {
//			int a = *itr;
//		}
//		totalTime += endTimer("multiset iterate", false);
//	}
//	std::cout << "multiset iterate: " << totalTime / LOOPS << "ms, fps: " << 1 / (totalTime / LOOPS / 1000) << std::endl;
//
//	totalTime = 0;
//	for (int i = 0; i < LOOPS; i++) {
//		startTimer();
//		// Displaying set elements
//		for (int i = 0; i < 1000; i++) {
//			S.insert(-i);
//		}
//		totalTime += endTimer("multiset insert", false);
//	}
//	startTimer();
//	itr = S.lower_bound(1000);
//	while (*itr != 1010) {
//		itr++;
//	}
//	endTimer("lowerBound", true);
//	std::cout << "lowerBoundVal:" << *itr << std::endl;
//
//	std::cout << "multiset insert: " << totalTime / LOOPS << "ms, fps: " << 1 / (totalTime / LOOPS / 1000) << "\n\n";
//}
//
//struct ShapePoint {
//	bool left;
//	glm::vec2 pt;
//	ShapeObj* parent;
//	int index;
//	ShapePoint() {
//		left = 0;
//		pt = glm::vec2(0, 0);
//		parent = nullptr;
//	}
//};
//
//static bool sortPtsByX(ShapePoint a, ShapePoint b) {
//	return a.pt.x < b.pt.x;
//}
//
//bool operator<(ShapePoint a, ShapePoint b) {
//	return a.pt.x < b.pt.x;
//}
//
//void vectorTest() {
//	srand(time(NULL));
//	std::vector<ShapeObj> shapeObjs;
//	const int SIZE = 30000;
//	for (int i = 0; i < SIZE; i++) {
//		ShapeObj s;
//		float x = ((1.0f * rand()) / RAND_MAX) * 4000.0f;
//		float y = ((1.0f * rand()) / RAND_MAX) * 4000.0f;
//		//std::cout << x << std::endl;
//		s.boundingBox = BoundingBox(y + 10, x + 10, y, x);
//		shapeObjs.push_back(s);
//	}
//
//	startTimer();
//	std::multiset<ShapePoint> mpts;
//	ShapePoint pt;
//	for (int i = 0; i < 200; i++) {
//		pt.left = true;
//		pt.pt.x = shapeObjs[i].boundingBox.left();
//		pt.pt.y = shapeObjs[i].boundingBox.bottom();
//		pt.parent = &shapeObjs[i];
//		mpts.insert(pt);
//		pt.left = false;
//		pt.pt.x = shapeObjs[i].boundingBox.right();
//		pt.pt.y = shapeObjs[i].boundingBox.top();
//		mpts.insert(pt);
//	}
//	endTimer("multi: ", true);
//
//	startTimer();
//	std::vector<ShapePoint> pts;
//	pts.reserve(SIZE * 2);
//	for (int i = 0; i < shapeObjs.size(); i++) {	
//		pt.left = true;
//		pt.pt.x = shapeObjs[i].boundingBox.left();
//		pt.pt.y = shapeObjs[i].boundingBox.bottom();
//		pt.parent = &shapeObjs[i];
//		pts.push_back(pt);
//		pt.left = false;
//		pt.pt.x = shapeObjs[i].boundingBox.right();
//		pt.pt.y = shapeObjs[i].boundingBox.top();
//		pts.push_back(pt);
//	}
//	std::sort(pts.begin(), pts.end(), sortPtsByX);
//	endTimer("vector: ", true);
//
//
//
//	std::vector<ShapePoint> scanline;
//	std::set<ShapeObj*> deleteList;
//	int erasedCount = 0;
//	int leftPts = 0;
//	int rightPts = 0;
//	for (int i = 0; i < pts.size(); i++) {
//		if (pts[i].left == false) { // Delete left points:
//			rightPts++;
//			for (int j = 0; j < scanline.size(); j++) {
//				if (scanline[j].parent == pts[i].parent) {
//					scanline.erase(scanline.begin() + j);
//					//std::cout << "erased" << std::endl;
//					erasedCount++;
//					break;
//				}
//			}
//			continue;
//		}
//		leftPts++;
//
//		// BB overlap check:
//		bool overlap = false;
//		for (int j = 0; j < scanline.size(); j++) {
//			if (pts[i].parent->boundingBox.top() < scanline[j].parent->boundingBox.bottom() &&
//				pts[i].parent->boundingBox.bottom() > scanline[j].parent->boundingBox.top()) {
//				deleteList.insert(scanline[j].parent);
//			}
//		}
//
//		// Add left Points:
//		scanline.push_back(pts[i]);
//		for (ShapePoint s : scanline) {
//			//std::cout << s.index << std::endl;
//		}
//		//std::cout << scanline .size()<< std::endl;
//		//std::cout << std::endl;
//	}
//	std::cout << "erase count: " << erasedCount << std::endl;
//	std::cout << "leftPts: " << leftPts << std::endl;
//	std::cout << "rightPts: " << rightPts << std::endl;
//
//}

void testing();