#include "renderSystem.hpp"

#include "vertex.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

RenderSystem::RenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout descriptorSetLayout)
	: m_device(device) {
	createGraphicsPipeline(renderPass, descriptorSetLayout);
	createUniformBuffers();
}

VkDescriptorBufferInfo RenderSystem::bufferDescriptor(uint32_t currentFrame) {
	VkDescriptorBufferInfo bufferInfo{};
	bufferInfo.buffer = m_uniformBuffers[currentFrame];
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(UniformBufferObject);

	return bufferInfo;
}

void RenderSystem::createGraphicsPipeline(VkRenderPass renderPass, VkDescriptorSetLayout descriptorSetLayout) {
	PipelineInfo pipelineInfo{};
	pipelineInfo.extent = m_swapchain->extent();
	pipelineInfo.descriptorSetLayout = &descriptorSetLayout;
	pipelineInfo.renderPass = renderPass;

	m_graphicsPipeline = std::make_unique<Pipeline>(m_device, m_pathVertexShader, m_pathFragmentShader, pipelineInfo);
};

void RenderSystem::createUniformBuffers() {
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	m_uniformBuffers.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);
	m_uniformBuffersMemory.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);

	for(size_t i = 0; i != Swapchain::MAX_FRAMES_IN_FLIGHT; ++i) {
		m_device.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffers[i], m_uniformBuffersMemory[i]);
	}
}

void RenderSystem::renderObjects(uint32_t currentImage, VkCommandBuffer commandBuffer, VkExtent2D frameExtent, VkDescriptorSet descriptorSet, std::vector<Object> objects) {
	m_graphicsPipeline->bind(commandBuffer);
	vkCmdBindDescriptorSets(
			commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_graphicsPipeline->layout(), 0, 1,
			&descriptorSet, 0, nullptr
	);

	for(auto o : objects) {
		o.bind(commandBuffer);
		updateUniformBuffer(currentImage, {0.0f, 0.0f, 0.0f});
		o.draw(commandBuffer);
	}
}

void RenderSystem::updateUniformBuffer(uint32_t currentImage, glm::vec3 offset) {
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), m_renderer.swapchainExtent().width / static_cast<float>(m_renderer.swapchainExtent().height), 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;  // Invert the y-coordinate of clip coordinate because glm was designed for OpenGL
	ubo.offset = offset;

	// NOTE: More efficient way to do this is by using push constants.
	void* data = nullptr;
	vkMapMemory(m_device.device(), m_uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(m_device.device(), m_uniformBuffersMemory[currentImage]);
}

RenderSystem::~RenderSystem() {
	for(size_t i = 0; i != Swapchain::MAX_FRAMES_IN_FLIGHT; ++i) {
		vkDestroyBuffer(m_device.device(), m_uniformBuffers[i], nullptr);
		vkFreeMemory(m_device.device(), m_uniformBuffersMemory[i], nullptr);
	}
}