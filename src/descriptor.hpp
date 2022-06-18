#pragma once

// Overview:
// Descriptors: Contain information about resources (Pointer to resource, resource type, size, etc).
// Sets:        Combine descriptors that are similar to better efficiency (texture descriptors in a set, UBO descriptors in a set, etc).
// SetLayouts:  Defines how many sets, what kind of descriptors the sets contain and at which bindings the descriptors are in the sets.
//
// Descriptor Pool is used to efficiently allocate memory for the descriptor sets.

#include <vulkan/vulkan.hpp>
#include "device.hpp"

class DescriptorSetLayout {
public:
	DescriptorSetLayout(Device& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>& bindings);
	~DescriptorSetLayout();

	VkDescriptorSetLayout layout() const { return m_descriptorSetLayout; }

private:
	// Owned by application
	Device& m_device;

	VkDescriptorSetLayout m_descriptorSetLayout;
	std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings;
};


class DescriptorPool {
public:
	DescriptorPool(Device& device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags,
				   const std::vector<VkDescriptorPoolSize>& poolSizes);
	~DescriptorPool();

	bool allocateDescriptorSet(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;
	void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;
	void resetPool();

private:
	// Owned by application
	Device& m_device;

	VkDescriptorPool m_descriptorPool;
};