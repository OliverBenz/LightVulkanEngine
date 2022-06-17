#include "renderer.hpp"

Renderer::Renderer(Device &device, Window &window) : m_device(device), m_window(window) {
	createGraphicsPipeline();
}


uint32_t Renderer::currentImageIndex() const {
	return m_currentImageIndex;
}

VkCommandBuffer Renderer::commandBuffer() const {
	return m_commandBuffers[m_swapchain->currentFrame()];
}

void Renderer::recreateSwapchain() {
	// TODO: Pass old swapchain to new object to be copied and then delete it.
	// Swapchain* oldSwapchain = m_swapchain.release();
	// m_swapchain.reset(nullptr);
	// m_swapchain = std::make_unique<Swapchain>(m_window, m_device, oldSwapchain);

	m_window.stopWhileMinimized();
	vkDeviceWaitIdle(m_device.device());

	m_swapchain.reset(nullptr);
	m_swapchain = std::make_unique<Swapchain>(m_window, m_device);

	m_graphicsPipeline.reset(nullptr);
	createGraphicsPipeline();
}

void Renderer::createGraphicsPipeline() {
	PipelineInfo pipelineInfo{};
	pipelineInfo.extent = m_swapchain->extent();
	pipelineInfo.descriptorSetLayout = &m_descriptorSetLayout;
	pipelineInfo.renderPass = m_swapchain->renderPass();

	m_graphicsPipeline = std::make_unique<Pipeline>(m_device,m_pathVertexShader,
	                                                m_pathFragmentShader, pipelineInfo);
};

void Renderer::createCommandBuffers() {
	m_commandBuffers.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_device.commandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

	if(vkAllocateCommandBuffers(m_device.device(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate command buffers!");
	}
}

void Renderer::beginFrame() {
	// Get next swapchain image
	m_currentImageIndex = -1;
	const VkResult result = m_swapchain->getNextImage(m_currentImageIndex);

	if(result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapchain();
		return;
	} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		// Suboptimal swap chain is also ok.. Recreate after presenting the image.
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	vkResetCommandBuffer(m_commandBuffers[m_swapchain->currentFrame()], 0);
}

void Renderer::endFrame() {
	// Submit command buffer
	const VkResult result = m_swapchain->submitCommandBuffer(m_commandBuffers[m_swapchain->currentFrame()], m_currentImageIndex);
	if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window.resized()) {
		recreateSwapchain();
	} else if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to present swap chain image!");
	}
}