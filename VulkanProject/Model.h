#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "CoordinateWrappers.h"

#ifndef TINY_OBJ_LOADED
#include "tiny_obj_loader.h"
#endif

struct Model {
	std::vector<Vertex> verts;
	std::vector<int> indices;
	int indexOffset;

	Model(const std::string modelPath) {
		loadModel(modelPath);
		indexOffset = 0;
	}
	bool loadModel(const std::string modelPath);
	void pushModelToVulkan();
};

