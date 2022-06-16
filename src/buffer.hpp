#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>

#include "device.hpp"
#include "object.hpp"

class Buffer {
public:
	// TODO: More objects
	Buffer(Device& device, Object& object);
	~Buffer();

	//! Bind the buffers of an object to a command Buffer
	void bind(VkCommandBuffer commandBuffer) {
		VkBuffer vertexBuffers[] = {m_vertexBuffer};
		VkDeviceSize offsets[] = {0};

		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}

	void draw(VkCommandBuffer commandBuffer) {
		vkCmdDrawIndexed(commandBuffer, m_object.indexCount(), 1, 0, 0, 0);
	}



private:
	void createVertexBuffer();
	void createIndexBuffer();

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
					  VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

private:
	// Owned by application
	Device& m_device;
	// Owned by renderer
	Object& m_object;

	// TODO: For multiple objects
	VkBuffer m_vertexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory m_vertexBufferMemory = VK_NULL_HANDLE;

	VkBuffer m_indexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory m_indexBufferMemory = VK_NULL_HANDLE;


};
