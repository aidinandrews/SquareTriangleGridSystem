#include "Model.h"

bool Model::loadModel(const std::string modelPath) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str())) {
		std::cout << warn + err << std::endl;
		return false;
	}
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			if (attrib.texcoords.size() > 0) {
				vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				attrib.texcoords[2 * index.texcoord_index + 1]
				};
			}

			// Temp:
			vertex.color = { float(rand()) / RAND_MAX,  float(rand()) / RAND_MAX,  float(rand()) / RAND_MAX };

			bool insideVertList = false;
			int vertIndex = 0;
			for (; vertIndex < verts.size(); vertIndex++) {
				if (verts[vertIndex] == vertex) {
					insideVertList = true;
					break;
				}
			}
			if (!insideVertList) {
				indices.push_back((int)verts.size());
				verts.push_back(vertex);
			}
			else {
				indices.push_back(vertIndex);
			}
		}
	}
	return true;
}