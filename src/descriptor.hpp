#pragma once

// Overview:
// Descriptors: Contain information about resources (Pointer to resource, resource type, size, etc).
// Sets:        Combine descriptors that are similar to better efficiency (texture descriptors in a set, UBO descriptors in a set, etc).
// SetLayouts:  Defines how many sets, what kind of descriptors the sets contain and at which bindings the descriptors are in the sets.
//
// Descriptor Pool is used to efficiently allocate memory for the descriptor sets.
// In here, integrated "Builder" classes are used to help easily construct the classes.
//     Otherwise, we would have to construct structs before calling the class constructors, which is a little less pretty :-)

#include <vulkan/vulkan.hpp>
#include <memory>

#include "device.hpp"


// ----- Descriptor Set Layout -----
class DescriptorSetLayout {
public:
	class Builder {
	public:
		Builder(LveDevice &lveDevice);

		Builder &addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count = 1);
		std::unique_ptr<DescriptorSetLayout> build() const;

	private:
		Device& m_device;
		std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings{};
	};

	DescriptorSetLayout(Device& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
	~DescriptorSetLayout();

	VkDescriptorSetLayout descriptorSetLayout() const;

private:
	// Owned by application
	Device& m_device;

	VkDescriptorSetLayout m_descriptorSetLayout;
	std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings;

	friend DescriptorWriter;
};


// ----- Descriptor Pool -----
class DescriptorPool {
public:
	//! Helper class to construct the pool sizes and create a descriptor pool.
	class Builder {
	public:
		Builder(Device &device);

		Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
		Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags); // Note: Flags not used for now.
		Builder& setMaxSets(uint32_t count);
		std::unique_ptr<LveDescriptorPool> build() const;

	private:
		Device& m_device;
		std::vector<VkDescriptorPoolSize> m_poolSizes{};
		uint32_t m_maxSets = 1000;
		VkDescriptorPoolCreateFlags m_poolFlags = 0;
	};


	DescriptorPool(Device& device, uint32_t maxSets, const std::vector<VkDescriptorPoolSize>& poolSizes);
	~DescriptorPool();

	bool allocateDescriptorSet(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;
	void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;
	void resetPool();

private:
	// Owned by application
	Device& m_device;

	VkDescriptorPool m_descriptorPool;

	friend DescriptorWriter;
};


// ----- Descriptor Writer -----
class DescriptorWriter {
public:
	DescriptorWriter(DescriptorSetLayout& setLayout, DescriptorPool& pool);

	DescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
	DescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

	bool build(VkDescriptorSet& set);
	void overwrite(VkDescriptorSet& set);

private:
	DescriptorSetLayout& m_setLayout;
	DescriptorPool& m_pool;
	std::vector<VkWriteDescriptorSet> m_writes;
};