#include "renderSystem.hpp"
#include "lwEngine/swapchain.hpp"
#include "lwEngine/vertex.hpp"

RenderSystem::RenderSystem(Device& device, VkRenderPass renderPass)
		: m_device(device) {
	createGraphicsPipeline(renderPass);
}

void RenderSystem::createGraphicsPipeline(VkRenderPass renderPass) {
	PipelineInfo pipelineInfo{};
	pipelineInfo.descriptorSetLayout = nullptr;
	pipelineInfo.renderPass = renderPass;

	m_graphicsPipeline = std::make_unique<Pipeline>(m_device, m_pathVertexShader, m_pathFragmentShader, pipelineInfo);
};

void RenderSystem::staticDraw(VkCommandBuffer commandBuffer, const Triangle& triangle) {
	m_graphicsPipeline->bind(commandBuffer);
	vkCmdBindDescriptorSets(
			commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_graphicsPipeline->layout(), 0, 1,
			nullptr, 0, nullptr
	);

	triangle.draw(commandBuffer);
}