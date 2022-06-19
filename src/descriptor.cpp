/* Based on blurrypiano's "littleVulkanEngine"
 * https://github.com/blurrypiano/littleVulkanEngine/blob/master/littleVulkanEngine/tutorial22/lve_buffer.hpp
 * Published under the MIT License (accessed 18.06.2022)
 */

#include "descriptor.hpp"

// ----- Descriptor Pool Builder -----
DescriptorPool::Builder::Builder(Device &device) : m_device{device} {
}

DescriptorPool::Builder& DescriptorPool::Builder::addPoolSize(VkDescriptorType descriptorType, uint32_t count) {
	m_poolSizes.push_back({descriptorType, count});
	return *this;
}

DescriptorPool::Builder& DescriptorPool::Builder::setPoolFlags(VkDescriptorPoolCreateFlags flags) {
	m_poolFlags = flags;
	return *this;
}

DescriptorPool::Builder& DescriptorPool::Builder::setMaxSets(uint32_t count) {
	m_maxSets = count;
	return *this;
}

std::unique_ptr<DescriptorPool> DescriptorPool::Builder::build() const {
	return std::make_unique<DescriptorPool>(m_device, m_maxSets, m_poolSizes);
}


// ----- Descriptor Pool -----
DescriptorPool::DescriptorPool(Device& device, uint32_t maxSets, const std::vector<VkDescriptorPoolSize>& poolSizes) : m_device(device) {
	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = maxSets;

	if(vkCreateDescriptorPool(m_device.device(), &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create descriptor pool!");
	}
}

bool DescriptorPool::allocateDescriptorSet(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const {
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_descriptorPool;
	allocInfo.pSetLayouts = &descriptorSetLayout;
	allocInfo.descriptorSetCount = 1;

	// Might want to create a "DescriptorPoolManager" class that handles this case, and builds
	// a new pool whenever an old pool fills up. But this is beyond our current scope
	if (vkAllocateDescriptorSets(m_device.device(), &allocInfo, &descriptor) != VK_SUCCESS) {
		// TODO: logMessage(LogLevel::Error, "Failed to allocate descriptor sets!");
		return false;
	}
	return true;
}

void DescriptorPool::freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const {
	vkFreeDescriptorSets(
			m_device.device(),
			m_descriptorPool,
			static_cast<uint32_t>(descriptors.size()),
			descriptors.data());
}

void DescriptorPool::resetPool() {
	vkResetDescriptorPool(m_device.device(), m_descriptorPool, 0);
}

DescriptorPool::~DescriptorPool() {
	vkDestroyDescriptorPool(m_device.device(), m_descriptorPool, nullptr);
}



// ----- Descriptor Set Layout Builder -----
DescriptorSetLayout::Builder::Builder(Device& device) : m_device(device) {
}

DescriptorSetLayout::Builder& DescriptorSetLayout::Builder::addBinding(uint32_t binding, VkDescriptorType descriptorType,
		VkShaderStageFlags stageFlags, uint32_t count)
{
	VkDescriptorSetLayoutBinding layoutBinding{};
	layoutBinding.binding = binding;
	layoutBinding.descriptorType = descriptorType;
	layoutBinding.descriptorCount = count;
	layoutBinding.stageFlags = stageFlags;

	m_bindings[binding] = layoutBinding;

	return *this;
}

std::unique_ptr<DescriptorSetLayout> DescriptorSetLayout::Builder::build() const {
	return std::make_unique<DescriptorSetLayout>(m_device, m_bindings);
}

// ----- Descriptor Set Layout -----
DescriptorSetLayout::DescriptorSetLayout(Device& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
	: m_device(device), m_bindings(bindings) {
	// Write unordered_map to vector.
	std::vector<VkDescriptorSetLayoutBinding> layoutBindings{};
	for (auto kv : bindings) {
		layoutBindings.push_back(kv.second);
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
	layoutInfo.pBindings = layoutBindings.data();

	if(vkCreateDescriptorSetLayout(m_device.device(), &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create descriptor set layout!");
	}
}

DescriptorSetLayout::~DescriptorSetLayout() {
	vkDestroyDescriptorSetLayout(m_device.device(), m_descriptorSetLayout, nullptr);
}

VkDescriptorSetLayout DescriptorSetLayout::descriptorSetLayout() const {
	return m_descriptorSetLayout;
}


// ----- Descriptor Writer -----
DescriptorWriter::DescriptorWriter(DescriptorSetLayout& setLayout, DescriptorPool& pool) : m_setLayout(setLayout), m_pool(pool) {
}

DescriptorWriter& DescriptorWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo) {
	auto& bindingDescription = m_setLayout.m_bindings.at(binding);

	VkWriteDescriptorSet descriptorWrite{};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstBinding = binding;
	descriptorWrite.descriptorType = bindingDescription.descriptorType;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = bufferInfo;
	// descriptorWrite.dstSet = m_descriptorSets[i];  // NOTE: Set in overwrite function.

	m_writes.push_back(descriptorWrite);
	return *this;
}

DescriptorWriter& DescriptorWriter::writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo) {
	auto &bindingDescription = m_setLayout.m_bindings.at(binding);

	VkWriteDescriptorSet descriptorWrite{};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstBinding = binding;
	descriptorWrite.descriptorType = bindingDescription.descriptorType;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pImageInfo = imageInfo;
	// descriptorWrite.dstSet = m_descriptorSets[i];  // NOTE: Set in overwrite function.

	m_writes.push_back(descriptorWrite);
	return *this;
}

bool DescriptorWriter::build(VkDescriptorSet& set) {
	// Try to allocate the memory for this descriptor.
	if(!m_pool.allocateDescriptorSet(m_setLayout.descriptorSetLayout(), set)) {
		return false;
	}

	// Write data to this descriptor
	overwrite(set);
	return true;
}

void DescriptorWriter::overwrite(VkDescriptorSet& set) {
	// Set is added here because write functions can be called multiple times and all writes will be to same buffer.
	//     so we don't have to specify for every write call if it's the same anyways.
	for (auto& write : m_writes) {
		write.dstSet = set;
	}

	vkUpdateDescriptorSets(m_pool.m_device.device(), static_cast<uint32_t>(m_writes.size()), m_writes.data(), 0, nullptr);
}
