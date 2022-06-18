#include "application.hpp"

#include <vector>

void Application::createDescriptorSets(RenderSystem& rotationSystem) {
    m_descriptorSets.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);

	// Create the two descriptor sets
	for (int i = 0; i != m_descriptorSets.size(); ++i) {
		// TODO: Move to buffer?
		auto imageInfo = m_modelViking.descriptorInfo();
		auto bufferInfo = rotationSystem.bufferDescriptor(static_cast<uint32_t>(i));

		// Create descriptor set with two descriptors from the specified pool with the specified layout.
		// TODO: Add assertions that the bindings are the same as in the layout!
		DescriptorWriter(*m_descriptorSetLayout, *m_descriptorPool)
			.writeBuffer(0, &bufferInfo)
			.writeImage(1, &imageInfo)
			.build(m_descriptorSets[i]);
    }
}

void Application::run() {
	m_descriptorSetLayout = DescriptorSetLayout::Builder(m_device)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1)
			.addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
			.build();

	RenderSystem rotationSystem{m_device, m_renderer.swapchainRenderPass(), m_descriptorSetLayout->descriptorSetLayout()};
	std::vector<Object> rotationObjects{m_modelViking};

	m_descriptorPool = DescriptorPool::Builder(m_device)
			.setMaxSets(static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT))
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, Swapchain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, Swapchain::MAX_FRAMES_IN_FLIGHT)
			.build();

	createDescriptorSets(rotationSystem);

	while(!m_window.shouldClose()) {
        glfwPollEvents();

		// Start rendering
		VkCommandBuffer commandBuffer = m_renderer.beginFrame();
		if(commandBuffer) {
			m_renderer.beginSwapchainRenderPass(commandBuffer);

			// Rendering ouf stuff
			m_modelViking.bind(commandBuffer);
			rotationSystem.renderObjects(m_renderer.currentSwapchainFrame(), commandBuffer,
										 m_renderer.swapchainExtent(), m_descriptorSets[m_renderer.currentSwapchainFrame()],
										 rotationObjects);
			m_modelViking.draw(commandBuffer);

			// End rendering
			m_renderer.endSwapchainRenderPass(commandBuffer);
			m_renderer.endFrame();
		}
    }

    vkDeviceWaitIdle(m_device.device());
}
