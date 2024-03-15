#pragma once
#include <iostream>
#include <array>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#include "ShapeObject.h"

struct Node {
	int key, height;
	int* left, right, next;
};

class AVLtree {
	Node* root;

public:
	AVLtree();
	~AVLtree();

	void insert(Node node);
	void remove(Node node);
	void find(Node node);
};