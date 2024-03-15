#include "Test.h"

bool operator<(XBound a, XBound b) {
	return a.val < b.val;
}

void itrCheck() {
	ObjManager2 objManager;
	std::vector<ShapeObj> shapes;
	std::multiset<XBound> XBounds;
	int width = 3000, height = 10;
	for (int i = 0; i < height; i++) {
		ShapeObj s = objManager.m_shapeObjLibrary[SHAPE_TYPE_TRIANGLE];
		s.move(XXYY(0, 0, 16, 0) * i);
		for (int j = 0; j < width; j++) {
			if (j % 3 == 0) {
				s.color = glm::vec3(1, 1, 0);
				s.moved = true;
				s.move(XXYY(10, 0, 0, 0));
			}
			else {
				s.color = glm::vec3(1, 1, 1);
				s.moved = false;
				s.move(XXYY(16, 0, 0, 0));
			}
			shapes.push_back(s);

			XBound xb;
			xb.type = BOUND_TYPE_LEFT;
			xb.val = s.boundingBox.left();
			xb.parent = &shapes[shapes.size() - 1];
			bool ass = xb.parent->moved;
			xb.moved = s.moved;
			xb.index = i*3000 + j;
			XBounds.insert(xb);
			xb.type = BOUND_TYPE_RIGHT;
			xb.val = s.boundingBox.right();
			XBounds.insert(xb);
		}
	}

	for (auto itr = XBounds.begin(); itr != XBounds.end(); itr++) {
		std::cout << shapes[itr->index].moved << std::endl;
	}
}

void testing() {
	std::cout << "start of tests" << std::endl;
	itrCheck();
	//multiSetTest();
	//setTest();
	//vectorTest();
	std::cout << "finished" << std::endl;
}