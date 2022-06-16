#include "model.hpp"

#include <exception>
#include <unordered_map>

#include "vertex.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "lib/tol/tiny_obj_loader.h"

Model::Model(const std::string pathModel, const std::string pathTexture) : m_pathModel(pathModel), m_pathTexture(pathTexture) {
	loadModel();
}

void Model::loadModel() {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, m_pathModel.c_str())) {
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};
			vertex.pos = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
			};
			vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};
			vertex.color = {1.0f, 1.0f, 1.0f};

			if (uniqueVertices.find(vertex) == uniqueVertices.end()) {
				uniqueVertices[vertex] = static_cast<uint32_t>(m_vertices.size());
				// m_vertices.push_back(vertex); TODO: Only using unique verices does not work for some reason..
			}
			m_vertices.push_back(vertex);
			m_indices.push_back(m_indices.size());
		}
	}
}
