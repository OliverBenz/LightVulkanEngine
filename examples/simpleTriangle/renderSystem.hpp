#pragma once

#include "lwEngine/device.hpp"
#include "lwEngine/pipeline.hpp"
#include "triangle.hpp"

#include <vulkan/vulkan.hpp>
#include <vector>
#include <memory>

//! Example render system that only does a static display.
class RenderSystem {
public:
	RenderSystem(Device& device, VkRenderPass renderPass);
	void staticDraw(VkCommandBuffer commandBuffer, const Triangle& object);

private:
	void createGraphicsPipeline(VkRenderPass renderPass);

private:
	// Owned by application
	Device& m_device;
	std::unique_ptr<Pipeline> m_graphicsPipeline;

	const std::string m_pathVertexShader = SHADER_PATH_VERTEX;
	const std::string m_pathFragmentShader = SHADER_PATH_FRAGMENT;
};