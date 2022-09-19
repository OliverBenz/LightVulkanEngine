#include "triangle.hpp"

Triangle::Triangle(Device& device) : Model(device, "", ""){
	m_hasIndexBuffer = true;

	createVertexBuffer(m_vertices);
}

void Triangle::draw(VkCommandBuffer commandBuffer) const {
	// We draw a static triangle so we don't need extra transformation steps between bind() and draw().
	// By calling the bind() inside the draw() function, we don't have to manually call it in the application.cpp.
	bind(commandBuffer);

	if(m_hasIndexBuffer) {
		vkCmdDrawIndexed(commandBuffer, m_indexBuffer->getInstanceCount(), 1, 0, 0, 0);
	} else {
		vkCmdDraw(commandBuffer, m_vertexBuffer->getInstanceCount(), 1, 0, 0);
	}
}