#pragma once
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

#define ORIENTATION_0   0
#define ORIENTATION_30  1
#define ORIENTATION_60  2
#define ORIENTATION_90	 3
#define ORIENTATION_120 4
#define ORIENTATION_150 5
#define ORIENTATION_180 6
#define ORIENTATION_210 7
#define ORIENTATION_240 8
#define ORIENTATION_270 9
#define ORIENTATION_300 10
#define ORIENTATION_330 11

#define ROTATE_0   0
#define ROTATE_30  1
#define ROTATE_60  2
#define ROTATE_90  3
#define ROTATE_120 4
#define ROTATE_150 5
#define ROTATE_180 6
#define ROTATE_210 7
#define ROTATE_240 8
#define ROTATE_270 9
#define ROTATE_300 10
#define ROTATE_330 11

struct Vertex {
	glm::vec3 pos;
	//glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoord;
	uint16_t modelTranformMatrixIndex;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R16_UINT;
		attributeDescriptions[3].offset = offsetof(Vertex, modelTranformMatrixIndex);

		return attributeDescriptions;
	}

	inline bool operator==(Vertex other) {
		bool samePos = this->pos == other.pos;
		bool sameTexPos = this->texCoord == other.texCoord;
		bool sameColor = this->color == other.color;
		return samePos && sameTexPos && sameColor;
	}
};

struct XXYY {
	// (x1 + x2, y1 + y2) = (x, y)
	int pos[4];

	XXYY();
	XXYY(int x1, int x2, int y1, int y2);

	float getXpos();
	float getYpos();
	glm::vec2 getXYpos();

	void setpos(int x1, int x2, int y1, int y2);
	void editpos(int x1, int x2, int y1, int y2);

	void print() {
		std::cout << "(" << pos[0] << ", " << pos[1] << ", " << pos[2] << ", " << pos[3] << ")\n";
	}

	inline XXYY operator+=(const XXYY A) {
		XXYY B;
		pos[0] += A.pos[0];
		pos[1] += A.pos[1];
		pos[2] += A.pos[2];
		pos[3] += A.pos[3];
		return B;
	}
	inline XXYY operator+(const XXYY A) {
		XXYY B;
		B.pos[0] = pos[0] + A.pos[0];
		B.pos[1] = pos[1] + A.pos[1];
		B.pos[2] = pos[2] + A.pos[2];
		B.pos[3] = pos[3] + A.pos[3];
		return B;
	}
	inline XXYY operator*(const XXYY A) {
		XXYY B;
		B.pos[0] = pos[0] * A.pos[0];
		B.pos[1] = pos[1] * A.pos[1];
		B.pos[2] = pos[2] * A.pos[2];
		B.pos[3] = pos[3] * A.pos[3];
		return B;
	}
	inline XXYY operator*(const int A) {
		XXYY B;
		B.pos[0] = pos[0] * A;
		B.pos[1] = pos[1] * A;
		B.pos[2] = pos[2] * A;
		B.pos[3] = pos[3] * A;
		return B;
	}
	inline XXYY operator/(const float A) {
		XXYY B;
		B.pos[0] = int(pos[0] / A);
		B.pos[1] = int(pos[1] / A);
		B.pos[2] = int(pos[2] / A);
		B.pos[3] = int(pos[3] / A);
		return B;
	}
	inline XXYY operator-(const XXYY A) {
		XXYY B;
		B.pos[0] = pos[0] - A.pos[0];
		B.pos[1] = pos[1] - A.pos[1];
		B.pos[2] = pos[2] - A.pos[2];
		B.pos[3] = pos[3] - A.pos[3];
		return B;
	}
	inline XXYY operator-=(const XXYY A) {
		XXYY B;
		pos[0] -= A.pos[0];
		pos[1] -= A.pos[1];
		pos[2] -= A.pos[2];
		pos[3] -= A.pos[3];
		return B;
	}
	inline bool operator==(const XXYY A) {
		if (this->pos[0] == A.pos[0] &&
			this->pos[1] == A.pos[1] &&
			this->pos[2] == A.pos[2] &&
			this->pos[3] == A.pos[3]) {
			return true;
		}
		return false;
	}
	inline bool operator!=(const XXYY A) {
		if (this->pos[0] != A.pos[0] &&
			this->pos[1] != A.pos[1] &&
			this->pos[2] != A.pos[2] &&
			this->pos[3] != A.pos[3]) {
			return true;
		}
		return false;
	}
};

float getDegrees(const int ORIENTATION);
float getRadians(const int ORIENTATION);

int getNextPtsIndex(XXYY point);
XXYY getNextAdj(XXYY currentAdj);