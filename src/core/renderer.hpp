#pragma once

#include <vulkan/vulkan.hpp>
#include <memory>
#include <vector>

#include "device.hpp"
#include "window.hpp"
#include "swapchain.hpp"

class Renderer {
public:
	Renderer(Device& device, Window& window);
	~Renderer();

	// Getters
	uint32_t currentImageIndex() const;
	uint32_t currentSwapchainFrame() const;
	VkCommandBuffer& commandBuffer();
	VkExtent2D swapchainExtent() const;
	VkRenderPass swapchainRenderPass() const;
	uint16_t maxFramesInFlight() const;

	VkCommandBuffer beginFrame();
	void endFrame();

	void beginSwapchainRenderPass(VkCommandBuffer commandBuffer);
	void endSwapchainRenderPass(VkCommandBuffer commandBuffer);

private:
	void createCommandBuffers();
	void recreateSwapchain();

private:
	// Owned by application
	Device& m_device;
	Window& m_window;

	std::unique_ptr<Swapchain> m_swapchain{std::make_unique<Swapchain>(m_window, m_device)};
	std::vector<VkCommandBuffer> m_commandBuffers;

	uint32_t m_currentImageIndex = static_cast<uint32_t>(-1);
};
