#include "CoordinateWrappers.h"

XXYY::XXYY() {
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;
	pos[3] = 0;
}

XXYY::XXYY(int x1, int x2, int y1, int y2) {
	pos[0] = x1;
	pos[1] = x2;
	pos[2] = y1;
	pos[3] = y2;
}

float XXYY::getXpos() {
	return (float(pos[0]) * 1.0f / 4.0f) + (float(pos[1]) * sqrt(3.0f) / 12.0f);
}

float XXYY::getYpos() {
	return (float(pos[2]) * 1.0f / 4.0f) + (float(pos[3]) * sqrt(3.0f) / 12.0f);
}

glm::vec2 XXYY::getXYpos() {
	return glm::vec2(getXpos(), getYpos());
}

void XXYY::setpos(int x1, int x2, int y1, int y2) {
	pos[0] = x1;
	pos[1] = x2;
	pos[2] = y1;
	pos[3] = y2;
}

void XXYY::editpos(int x1, int x2, int y1, int y2) {
	pos[0] += x1;
	pos[1] += x2;
	pos[2] += y1;
	pos[3] += y2;
}

const XXYY points[12] = {
   XXYY(4, 0, 0, 0),
   XXYY(0, 6, -2, 0),
   XXYY(2, 0, 0, -6),
   XXYY(0, 0, -4, 0),
   XXYY(-2, 0, 0, -6),
   XXYY(0, -6, -2, 0),
   XXYY(-4, 0, 0, 0),
   XXYY(0, -6, 2, 0),
   XXYY(-2, 0, 0, 6),
   XXYY(0, 0, 4, 0),
   XXYY(2, 0, 0, 6),
   XXYY(0, 6, 2, 0)
};

int getNextPtsIndex(XXYY point) {
	for (int i = 0; i < 12; i++) {
		if (point == points[i]) {
			return i;
		}
	}
	return -1;
}

XXYY getNextAdj(XXYY currentAdj) {
	if (currentAdj == XXYY(0, 0, 0, 0)) {
		std::cout << "XXYY rotation candidate has no length!" << std::endl;
		return XXYY(0, 0, 0, 0);
	}
	int multiply = 0;
	bool isFactor = false;
	for (int i = 0; i < 12; i++) {
		XXYY testXXYY = points[i];
		// Find a non 0 xxyy x or y:
		for (int j = 0; j < 4; j++) {
			if (testXXYY.pos[j] == 0) {
				continue;
			}
			multiply = currentAdj.pos[j] / testXXYY.pos[j];
			if (testXXYY * multiply == currentAdj) {
				XXYY nextUnitXXYY = points[(i + 1) % 12];
				return nextUnitXXYY * multiply;
			}
		}
	}
	currentAdj.print();
	std::cout << " is not rotateable!" << std::endl;
	return XXYY(0, 0, 0, 0);
}

float getDegrees(const int ORIENTATION) {
	return float(ORIENTATION) * 30.0f;
}

float getRadians(const int ORIENTATION) {
	return float(float(ORIENTATION) * (M_PI / 6.0f));
}