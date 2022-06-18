#pragma once

#include <vulkan/vulkan.hpp>
#include "device.hpp"
#include "window.hpp"
#include "swapchain.hpp"
#include "pipeline.hpp"
#include "model.hpp"

class Renderer {
public:
	Renderer(Device& device, Window& window, VkDescriptorSetLayout pipelineLayout);
	~Renderer();

	// Getters
	uint32_t currentImageIndex() const;
	uint32_t currentSwapchainFrame() const;
	VkCommandBuffer& commandBuffer();
	VkExtent2D swapchainExtent() const;

	VkCommandBuffer beginFrame();
	void endFrame();

	void beginSwapchainRenderPass(VkCommandBuffer commandBuffer);
	void endSwapchainRenderPass(VkCommandBuffer commandBuffer);

	// Bind one or more descriptor sets to the graphics pipeline.
	void bindDesriptorSet(VkDescriptorSet& descriptorSet);
	void bindDesriptorSets(std::vector<VkDescriptorSet>& descriptorSets);

private:
	void createGraphicsPipeline();
	void createCommandBuffers();
	void recreateSwapchain();

private:
	// Owned by application
	Device& m_device;
	Window& m_window;

	std::unique_ptr<Swapchain> m_swapchain{std::make_unique<Swapchain>(m_window, m_device)};
	std::unique_ptr<Pipeline> m_graphicsPipeline;
	std::vector<VkCommandBuffer> m_commandBuffers;

	uint32_t m_currentImageIndex = -1;

	VkDescriptorSetLayout m_descriptorSetLayout; // TODO: Move this somewhere proper

	// TODO: Move following outside of here. Is not engine dependent but an example...
	const std::string m_pathVertexShader = "../resources/shaders/vert.spv";
	const std::string m_pathFragmentShader = "../resources/shaders/frag.spv";
};
