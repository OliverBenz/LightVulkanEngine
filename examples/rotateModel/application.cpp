#include "application.hpp"
#include "renderSystem.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include <vector>

void Application::run() {
	// Create descriptor set layout
	std::unique_ptr<DescriptorSetLayout> descriptorSetLayout = DescriptorSetLayout::Builder(m_device)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1)
			.addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
			.build();

	// Create render systems
	RenderSystem rotationSystem{m_device, m_renderer.swapchainRenderPass(), descriptorSetLayout->descriptorSetLayout()};
	std::vector<Model*> rotationObjects = {&m_modelViking};

	// Create descriptor pool
	m_descriptorPool = DescriptorPool::Builder(m_device)
			.setMaxSets(static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT))
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, Swapchain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, Swapchain::MAX_FRAMES_IN_FLIGHT)
			.build();

	// Create two descriptor sets
	std::array<VkDescriptorSet, Swapchain::MAX_FRAMES_IN_FLIGHT> descriptorSets;
	for (int i = 0; i != descriptorSets.size(); ++i) {
		auto bufferInfo = rotationSystem.bufferDescriptor(static_cast<uint32_t>(i));
		auto imageInfo = m_modelViking.descriptorInfo();

		// Create descriptor set with two descriptors from the specified pool with the specified layout.
		DescriptorWriter(*descriptorSetLayout, *m_descriptorPool)
				.writeBuffer(0, &bufferInfo)
				.writeImage(1, &imageInfo)
				.build(descriptorSets[i]);
	}

	// Render loop
	while(!m_window.shouldClose()) {
        glfwPollEvents();

		// Start rendering
		VkCommandBuffer commandBuffer = m_renderer.beginFrame();
		if(commandBuffer) {
			m_renderer.beginSwapchainRenderPass(commandBuffer);

			// Rendering ouf stuff
			m_modelViking.bind(commandBuffer);
			rotationSystem.renderObjects(m_renderer.currentSwapchainFrame(), commandBuffer,
										 m_renderer.swapchainExtent(), descriptorSets[m_renderer.currentSwapchainFrame()],
										 rotationObjects);
			m_modelViking.draw(commandBuffer);

			// End rendering
			m_renderer.endSwapchainRenderPass(commandBuffer);
			m_renderer.endFrame();
		}
    }

    vkDeviceWaitIdle(m_device.device());
}
