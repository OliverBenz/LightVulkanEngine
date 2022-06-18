#include "renderer.hpp"

Renderer::Renderer(Device &device, Window &window, VkDescriptorSetLayout pipelineLayout) : m_device(device), m_window(window), m_descriptorSetLayout(pipelineLayout) {
	createGraphicsPipeline();
	createCommandBuffers();
}

uint32_t Renderer::currentImageIndex() const {
	return m_currentImageIndex;
}

uint32_t Renderer::currentSwapchainFrame() const {
	return m_swapchain->currentFrame();
}

VkCommandBuffer& Renderer::commandBuffer() {
	return m_commandBuffers[m_swapchain->currentFrame()];
}

VkExtent2D Renderer::swapchainExtent() const {
	return m_swapchain->extent();
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

VkCommandBuffer Renderer::beginFrame() {
	// Get next swapchain image
	m_currentImageIndex = -1;
	const VkResult result = m_swapchain->getNextImage(m_currentImageIndex);

	if(result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapchain();
		return nullptr;
	} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		// Suboptimal swap chain is also ok.. Recreate after presenting the image.
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	vkResetCommandBuffer(commandBuffer(), 0);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if(vkBeginCommandBuffer(commandBuffer(), &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("Failed to begin recording command buffer!");
	}

	return commandBuffer();
}

void Renderer::endFrame() {
	if(vkEndCommandBuffer(commandBuffer()) != VK_SUCCESS) {
		throw std::runtime_error("Failed to record command buffer!");
	}

	// Submit command buffer
	const VkResult result = m_swapchain->submitCommandBuffer(commandBuffer(), m_currentImageIndex);
	if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window.resized()) {
		recreateSwapchain();
	} else if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to present swap chain image!");
	}
}

void Renderer::beginSwapchainRenderPass(VkCommandBuffer commandBuffer) {
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_swapchain->renderPass();
	renderPassInfo.framebuffer = m_swapchain->frameBuffer(m_currentImageIndex);
	renderPassInfo.renderArea.offset = {0,0};
	renderPassInfo.renderArea.extent = m_swapchain->extent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
	clearValues[1].depthStencil = {1.0f, 0};

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	m_graphicsPipeline->bind(commandBuffer);
}

void Renderer::endSwapchainRenderPass(VkCommandBuffer commandBuffer) {
	vkCmdEndRenderPass(commandBuffer);
}

void Renderer::bindDesriptorSet(VkDescriptorSet& descriptorSet) {
	vkCmdBindDescriptorSets(
			commandBuffer(),
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_graphicsPipeline->layout(), 0, 1,
			&descriptorSet, 0, nullptr
	);
}

void Renderer::bindDesriptorSets(std::vector<VkDescriptorSet>& descriptorSets) {
	vkCmdBindDescriptorSets(
			commandBuffer(),
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_graphicsPipeline->layout(), 0, descriptorSets.size(),
			descriptorSets.data(), 0, nullptr
			);
}

Renderer::~Renderer() {

}