#pragma once

#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>

#include "device.hpp"

struct PipelineInfo {
	VkRenderPass renderPass;
	VkDescriptorSetLayout* descriptorSetLayout;
};

class Pipeline {
public:
	Pipeline(Device& device, const std::string& pathVertexFile, const std::string& pathFragmentFile, PipelineInfo& info);
	~Pipeline();

	VkPipelineLayout layout() const;
	void bind(VkCommandBuffer commandBuffer);

private:
	void createGraphicsPipeline(const std::string& pathVertexFile, const std::string& pathFragmentFile, const PipelineInfo& info);
	VkShaderModule createShadersModule(const std::vector<char>& code);

	static std::vector<char> readFile(const std::string& filename);

private:
	// Owned by application
	Device& m_device;

	VkPipelineLayout m_pipelineLayout;
	VkPipeline m_graphicsPipeline;
};
