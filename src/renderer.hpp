#pragma once

#include <vulkan/vulkan.hpp>
#include "device.hpp"
#include "window.hpp"
#include "swapchain.hpp"
#include "pipeline.hpp"
#include "model.hpp"

class Renderer {
public:
	Renderer(Device& device, Window& window);
	~Renderer();

	void beginFrame();
	void endFrame();

	uint32_t currentImageIndex() const;
	VkCommandBuffer commandBuffer() const;

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

	// TODO: Move following outside of here. Is not engine dependent but an example...
	const std::string m_pathVertexShader = "../resources/shaders/vert.spv";
	const std::string m_pathFragmentShader = "../resources/shaders/frag.spv";

	// Model
	Model m_modelViking{m_device, "../resources/models/viking_room.obj","../resources/textures/viking_room.png"};
};
