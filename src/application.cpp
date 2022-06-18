#include "application.hpp"

// For uniform buffer
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

Application::Application() {
	m_descriptorSetLayout = DescriptorSetLayout::Builder(m_device)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1)
			.addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
			.build();

	createUniformBuffers();

	m_descriptorPool = DescriptorPool::Builder(m_device)
			.setMaxSets(static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT))
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, Swapchain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, Swapchain::MAX_FRAMES_IN_FLIGHT)
			.build();

	createDescriptorSets();
}

void Application::createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    m_uniformBuffers.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);
    m_uniformBuffersMemory.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);

    for(size_t i = 0; i != Swapchain::MAX_FRAMES_IN_FLIGHT; ++i) {
        m_device.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffers[i], m_uniformBuffersMemory[i]);
    }
}

void Application::createDescriptorSets() {
    m_descriptorSets.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);

	// Create the two descriptor sets
	for (int i = 0; i != m_descriptorSets.size(); ++i) {
		// TODO: Move to buffer?
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = m_uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

		// TODO: Move to model.
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = m_modelViking.imageView();
        imageInfo.sampler = m_modelViking.sampler();

		// Create descriptor set with two descriptors.
		DescriptorWriter(*m_descriptorSetLayout, *m_descriptorPool)
			.writeBuffer(0, &bufferInfo)
			.writeImage(1, &imageInfo)
			.build(m_descriptorSets[i]);
    }
}

void Application::run() {
	while(!m_window.shouldClose()) {
        glfwPollEvents();

		// Start rendering
		VkCommandBuffer commandBuffer = m_renderer.beginFrame();
		if(commandBuffer) {
			m_renderer.beginSwapchainRenderPass(commandBuffer);

			m_renderer.bindDesriptorSet(m_descriptorSets[m_renderer.currentSwapchainFrame()]);

			// Rendering ouf stuff
			m_modelViking.bind(commandBuffer);
			updateUniformBuffer(m_renderer.currentSwapchainFrame(), {1.0f, 0.0f, 0.0f});
			m_modelViking.draw(commandBuffer);

			// End rendering
			m_renderer.endSwapchainRenderPass(commandBuffer);
			m_renderer.endFrame();
		}
    }

    vkDeviceWaitIdle(m_device.device());
}

void Application::updateUniformBuffer(uint32_t currentImage, glm::vec3 offset) {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), m_renderer.swapchainExtent().width / static_cast<float>(m_renderer.swapchainExtent().height), 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;  // Invert the y-coordinate of clip coordinate because glm was designed for OpenGL
	ubo.offset = offset;

    // NOTE: More efficient way to do this is by using push constants.
    void* data = nullptr;
    vkMapMemory(m_device.device(), m_uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(m_device.device(), m_uniformBuffersMemory[currentImage]);
}

Application::~Application() {
    for(size_t i = 0; i != Swapchain::MAX_FRAMES_IN_FLIGHT; ++i) {
        vkDestroyBuffer(m_device.device(), m_uniformBuffers[i], nullptr);
        vkFreeMemory(m_device.device(), m_uniformBuffersMemory[i], nullptr);
    }
}
