#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <optional>
#include <memory>

#include "device.hpp"
#include "window.hpp"
#include "swapchain.hpp"
#include "vertex.hpp"

// TODO: Add chapters
//  - Callback messages (52ff)
//  - Debugging instance creation and destruction (57ff)
class TriangleApp {
public:
    TriangleApp();
    ~TriangleApp();

    void run();

private:
    void initWindow();

    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    VkShaderModule createShadersModule(const std::vector<char>& code);

    void createVertexBuffer();
    void createIndexBuffer();
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void createSyncObjects();
    void drawFrame();
    void updateUniformBuffer(uint32_t currentImage);

    // Image
    void createTextureImage();
    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
        VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    void createTextureImageView();

    // Texture sampler
    void createTextureSampler();

	// Model
	void loadModel();

	// To be moved to renderer
	void recreateSwapchain();

private:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    Window m_window{WIDTH, HEIGHT, "Vulkan"};
    Device m_device{m_window};
	std::unique_ptr<Swapchain> m_swapchain;

    VkDescriptorPool m_descriptorPool;
    std::vector<VkDescriptorSet> m_descriptorSets;
    VkDescriptorSetLayout m_descriptorSetLayout;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_graphicsPipeline;

    // Buffers
    std::vector<VkCommandBuffer> m_commandBuffers;
    
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
    
    VkBuffer m_indexBuffer;
    VkDeviceMemory m_indexBufferMemory;

    std::vector<VkBuffer> m_uniformBuffers;
    std::vector<VkDeviceMemory> m_uniformBuffersMemory;

    // Image
    VkImage m_textureImage;
    VkDeviceMemory m_textureImageMemory;
    VkImageView m_textureImageView;

    // Texture Sampler
    VkSampler m_textureSampler;

	// Module
	const std::string m_pathModel = "../resources/models/viking_room.obj";
	const std::string m_pathTexture = "../resources/textures/viking_room.png";
	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
};