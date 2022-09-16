#pragma once

#include "lwEngine/device.hpp"
#include "lwEngine/pipeline.hpp"
#include "lwEngine/model.hpp"

#include <vulkan/vulkan.hpp>
#include <vector>
#include <memory>

//! Example render system that does simple transformation.
class RenderSystem {
public:
	RenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout descriptorSetLayout);
	~RenderSystem();

	void renderObjects(uint32_t currentFrame, VkCommandBuffer commandBuffer, VkExtent2D frameExtent, VkDescriptorSet descriptorSet, std::vector<Model*> objects);

	VkDescriptorBufferInfo bufferDescriptor(uint32_t currentFrame);

private:
	void createGraphicsPipeline(VkRenderPass renderPass, VkDescriptorSetLayout descriptorSetLayout);
	void createUniformBuffers();
	void updateUniformBuffer(uint32_t currentImage, VkExtent2D frameExtent, unsigned objectId);

private:
	// Owned by application
	Device& m_device;

	const std::string m_pathVertexShader = SHADER_PATH_VERTEX;
	const std::string m_pathFragmentShader = SHADER_PATH_FRAGMENT;

	std::vector<VkBuffer> m_uniformBuffers;
	std::vector<VkDeviceMemory> m_uniformBuffersMemory;
	std::unique_ptr<Pipeline> m_graphicsPipeline;
};