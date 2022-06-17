#include "application.hpp"

// For uniform buffer
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

Application::Application() {
	// NOTE: These two before the framebuffers in swapchain?
	createDescriptorSetLayout();
	createGraphicsPipeline();

	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
}

void Application::createDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;  // Only used in the vertex shader.
    uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings= {uboLayoutBinding, samplerLayoutBinding};

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 2;
    layoutInfo.pBindings = bindings.data();

    if(vkCreateDescriptorSetLayout(m_device.device(), &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor set layout!");
    }
}

void Application::createGraphicsPipeline() {
	PipelineInfo pipelineInfo{};
	pipelineInfo.extent = m_swapchain->extent();
	pipelineInfo.descriptorSetLayout = &m_descriptorSetLayout;
	pipelineInfo.renderPass = m_swapchain->renderPass();

	m_graphicsPipeline = std::make_unique<Pipeline>(m_device,m_pathVertexShader,
													m_pathFragmentShader, pipelineInfo);
};

void Application::createCommandBuffers() {
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

void Application::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_swapchain->renderPass();
    renderPassInfo.framebuffer = m_swapchain->frameBuffer(imageIndex);
    renderPassInfo.renderArea.offset = {0,0};
    renderPassInfo.renderArea.extent = m_swapchain->extent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	m_graphicsPipeline->bind(commandBuffer);
	// Bind vertex/index buffers to command buffer
	// TODO: Move currentFrame() function from swapchain to renderer class?
	m_modelViking.bind(commandBuffer);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline->layout(), 0, 1, &m_descriptorSets[m_swapchain->currentFrame()], 0, nullptr);

	updateUniformBuffer(m_swapchain->currentFrame(), {1.0f, 0.0f, 0.0f});
    m_modelViking.draw(commandBuffer);

	// TODO: Why is it only drawn once??
	updateUniformBuffer(m_swapchain->currentFrame(), {0.0f, 0.0f, 0.0f});
	m_modelViking.draw(commandBuffer);

    vkCmdEndRenderPass(commandBuffer);

    if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }
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

void Application::createDescriptorPool() {
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT);

    if(vkCreateDescriptorPool(m_device.device(), &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor pool!");
    }
}

void Application::createDescriptorSets() {
    std::vector<VkDescriptorSetLayout> layouts(Swapchain::MAX_FRAMES_IN_FLIGHT, m_descriptorSetLayout);

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    m_descriptorSets.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);
    if(vkAllocateDescriptorSets(m_device.device(), &allocInfo, m_descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate descriptor sets!");
    }

    for(size_t i = 0; i != Swapchain::MAX_FRAMES_IN_FLIGHT; ++i) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = m_uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = m_modelViking.imageView();
        imageInfo.sampler = m_modelViking.sampler();

        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = m_descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;
        descriptorWrites[0].pImageInfo = nullptr;
        descriptorWrites[0].pTexelBufferView = nullptr;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = m_descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pBufferInfo = nullptr;
        descriptorWrites[1].pImageInfo = &imageInfo;
        descriptorWrites[1].pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(m_device.device(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void Application::run() {
	while(!m_window.shouldClose()) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(m_device.device());
}

void Application::recreateSwapchain() {
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

void Application::drawFrame() {
	// Get next swapchain image
    uint32_t imageIndex = -1;
	VkResult result = m_swapchain->getNextImage(imageIndex);
	if(result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapchain();
		return;
	} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		// Suboptimal swap chain is also ok.. Recreate after presenting the image.
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	vkResetCommandBuffer(m_commandBuffers[m_swapchain->currentFrame()], 0);
	recordCommandBuffer(m_commandBuffers[m_swapchain->currentFrame()], imageIndex);

	// Submit command buffer
	result = m_swapchain->submitCommandBuffer(m_commandBuffers[m_swapchain->currentFrame()], imageIndex);
	if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window.resized()) {
		recreateSwapchain();
	} else if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to present swap chain image!");
	}
}

void Application::updateUniformBuffer(uint32_t currentImage, glm::vec3 offset) {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), m_swapchain->extent().width / static_cast<float>(m_swapchain->extent().height), 0.1f, 10.0f);
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

    vkDestroyDescriptorPool(m_device.device(), m_descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(m_device.device(), m_descriptorSetLayout, nullptr);
}
