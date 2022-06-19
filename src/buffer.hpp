#pragma once

#include "device.hpp"

class Buffer {
public:
	Buffer(Device& device, VkDeviceSize m_instanceSize, uint32_t m_instanceCount, VkBufferUsageFlags m_usageFlags,
			VkMemoryPropertyFlags m_memoryPropertyFlags, VkDeviceSize minOffsetAlignment = 1);
	~Buffer();

	VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
	void unmap();

	void writeToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
	VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
	VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
	VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

	void writeToIndex(void* data, int index);
	VkResult flushIndex(int index);
	VkDescriptorBufferInfo descriptorInfoForIndex(int index);
	VkResult invalidateIndex(int index);

	VkBuffer getBuffer() const;
	void* getMappedMemory() const;
	uint32_t getInstanceCount() const;
	VkDeviceSize getInstanceSize() const;
	VkDeviceSize getAlignmentSize() const;
	VkBufferUsageFlags getUsageFlags() const;
	VkMemoryPropertyFlags getMemoryPropertyFlags() const;
	VkDeviceSize getBufferSize() const;

private:
	static VkDeviceSize getAlignment(VkDeviceSize m_instanceSize, VkDeviceSize minOffsetAlignment);

	Device& m_device;
	void* m_mapped = nullptr;
	VkBuffer m_buffer = VK_NULL_HANDLE;
	VkDeviceMemory m_memory = VK_NULL_HANDLE;

	VkDeviceSize m_bufferSize;
	uint32_t m_instanceCount;
	VkDeviceSize m_instanceSize;
	VkDeviceSize m_alignmentSize;
	VkBufferUsageFlags m_usageFlags;
	VkMemoryPropertyFlags m_memoryPropertyFlags;
};

