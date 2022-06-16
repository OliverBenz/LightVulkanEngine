#pragma once

#include <vector>
#include "vertex.hpp"

class Object {
public:
	//! Returns size of vertices in bytes.
	uint64_t vertexSize() {
		return sizeof(m_vertices[0]) * m_vertices.size();
	}

	//! Returns size of indices in bytes.
	uint64_t indexSize() {
		return sizeof(m_indices[0]) * m_indices.size();
	}

	//! Returns number of indices.
	uint32_t indexCount() {
		return static_cast<uint32_t>(m_indices.size());
	}

	Vertex* vertices() {
		return m_vertices.data();
	}

	uint32_t* indices() {
		return m_indices.data();
	}

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
};