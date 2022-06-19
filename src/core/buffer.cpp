/* Based on blurrypiano's "littleVulkanEngine"
 * https://github.com/blurrypiano/littleVulkanEngine/blob/master/littleVulkanEngine/tutorial22/lve_buffer.hpp
 * Published under the MIT License (accessed 17.06.2022)
 */

#include "buffer.hpp"

#include <cassert>
#include <cstring>

/*! Returns the minimum instance size required to be compatible with devices minOffsetAlignment.
 *  @param m_instanceSize The size of an instance.
 *  @param minOffsetAlignment The minimum required alignment, in bytes, for the offset member (eg minUniformBufferOffsetAlignment).
 *  @return VkResult of the buffer mapping call.
 */
VkDeviceSize Buffer::getAlignment(VkDeviceSize m_instanceSize, VkDeviceSize minOffsetAlignment) {
	if (minOffsetAlignment > 0) {
		return (m_instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
	}
	return m_instanceSize;
}

Buffer::Buffer(Device &device, VkDeviceSize instanceSize, uint32_t instanceCount,
		VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment)
		: m_device{device}, m_instanceSize{instanceSize}, m_instanceCount{instanceCount},
		  m_usageFlags{usageFlags}, m_memoryPropertyFlags{memoryPropertyFlags}
{
	m_alignmentSize = getAlignment(m_instanceSize, minOffsetAlignment);
	m_bufferSize = m_alignmentSize * m_instanceCount;
	m_device.createBuffer(m_bufferSize, m_usageFlags, m_memoryPropertyFlags, m_buffer, m_memory);
}

// Getters
VkBuffer Buffer::getBuffer() const {
	return m_buffer;
}

void* Buffer::getMappedMemory() const {
	return m_mapped;
}

uint32_t Buffer::getInstanceCount() const {
	return m_instanceCount;
}

VkDeviceSize Buffer::getInstanceSize() const {
	return m_instanceSize;
}

VkDeviceSize Buffer::getAlignmentSize() const {
	return m_instanceSize;
}

VkBufferUsageFlags Buffer::getUsageFlags() const {
	return m_usageFlags;
}

VkMemoryPropertyFlags Buffer::getMemoryPropertyFlags() const {
	return m_memoryPropertyFlags;
}

VkDeviceSize Buffer::getBufferSize() const {
	return m_bufferSize;
}

/** Map a memory range of this m_buffer. If successful, m_mapped points to the specified m_buffer range.
 *  @param size (Optional) Size of the memory range to map. Pass VK_WHOLE_SIZE to map the complete m_buffer range.
 *  @param offset (Optional) Byte offset from beginning.
 *  @return VkResult of the m_buffer mapping call.
 */
VkResult Buffer::map(VkDeviceSize size, VkDeviceSize offset) {
	assert(m_buffer && m_memory && "Called map on m_buffer before create");
	return vkMapMemory(m_device.device(), m_memory, offset, size, 0, &m_mapped);
}

/** Unmap a m_mapped memory range.
 *  @note Does not return a result as vkUnmapMemory can't fail.
 */
void Buffer::unmap() {
	if (m_mapped) {
		vkUnmapMemory(m_device.device(), m_memory);
		m_mapped = nullptr;
	}
}

/** Copies the specified data to the m_mapped m_buffer. Default value writes whole m_buffer range.
 *  @param data Pointer to the data to copy.
 *  @param size (Optional) Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete buffer range.
 *  @param offset (Optional) Byte offset from beginning of m_mapped region.
 */
void Buffer::writeToBuffer(void *data, VkDeviceSize size, VkDeviceSize offset) {
	assert(m_mapped && "Cannot copy to unmapped m_buffer");

	if (size == VK_WHOLE_SIZE) {
		memcpy(m_mapped, data, m_bufferSize);
	} else {
		char *memOffset = (char *)m_mapped;
		memOffset += offset;
		memcpy(memOffset, data, size);
	}
}

/** Flush a memory range of the m_buffer to make it visible to the device.
 *  @note Only required for non-coherent memory.
 *  @param size (Optional) Size of the memory range to flush. Pass VK_WHOLE_SIZE to flush the complete buffer range.
 *  @param offset (Optional) Byte offset from beginning.
 *  @return VkResult of the flush call.
 */
VkResult Buffer::flush(VkDeviceSize size, VkDeviceSize offset) {
	VkMappedMemoryRange mappedRange = {};
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = m_memory;
	mappedRange.offset = offset;
	mappedRange.size = size;

	return vkFlushMappedMemoryRanges(m_device.device(), 1, &mappedRange);
}

/** Invalidate a memory range of the m_buffer to make it visible to the host.
 *  @note Only required for non-coherent memory.
 *  @param size (Optional) Size of the memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate the complete m_buffer range.
 *  @param offset (Optional) Byte offset from beginning.
 *  @return VkResult of the invalidate call.
 */
VkResult Buffer::invalidate(VkDeviceSize size, VkDeviceSize offset) {
	VkMappedMemoryRange mappedRange = {};
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = m_memory;
	mappedRange.offset = offset;
	mappedRange.size = size;
	return vkInvalidateMappedMemoryRanges(m_device.device(), 1, &mappedRange);
}

/** Create a m_buffer info descriptor.
 *  @param size (Optional) Size of the memory range of the descriptor.
 *  @param offset (Optional) Byte offset from beginning.
 *  @return VkDescriptorBufferInfo of specified offset and range
 */
VkDescriptorBufferInfo Buffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
	return VkDescriptorBufferInfo{m_buffer, offset, size,};
}

/** Copies "m_instanceSize" bytes of data to the m_mapped m_buffer at an offset of index * m_alignmentSize.
 *  @param data Pointer to the data to copy.
 *  @param index Used in offset calculation.
 */
void Buffer::writeToIndex(void *data, int index) {
	writeToBuffer(data, m_instanceSize, index * m_alignmentSize);
}

/** Flush the memory range at index * m_alignmentSize of the m_buffer to make it visible to the device.
 *  @param index Used in offset calculation.
 */
VkResult Buffer::flushIndex(int index) {
	return flush(m_alignmentSize, index * m_alignmentSize);
}

/** Create a m_buffer info descriptor.
 *  @param index Specifies the region given by index * m_alignmentSize.
 *  @return VkDescriptorBufferInfo for instance at index.
 */
VkDescriptorBufferInfo Buffer::descriptorInfoForIndex(int index) {
	return descriptorInfo(m_alignmentSize, index * m_alignmentSize);
}

/** Invalidate a memory range of the m_buffer to make it visible to the host.
 *  @note Only required for non-coherent memory.
 *  @param index Specifies the region to invalidate: index * m_alignmentSize.
 *  @return VkResult of the invalidate call.
 */
VkResult Buffer::invalidateIndex(int index) {
	return invalidate(m_alignmentSize, index * m_alignmentSize);
}

Buffer::~Buffer() {
	unmap();
	vkDestroyBuffer(m_device.device(), m_buffer, nullptr);
	vkFreeMemory(m_device.device(), m_memory, nullptr);
}