#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <optional>

#include "device.hpp"
#include "window.hpp"
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

    void createLogicalDevice();

    void createSwapChain();
    void cleanupSwapChain();
    void recreateSwapChain(); // If window surface changes for example..
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    
    void createImageViews();
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    VkShaderModule createShadersModule(const std::vector<char>& code);

    void createFramebuffers();
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

    // Depth Image
    void createDepthResources();
    VkFormat findDepthFormat();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	// Model
	void loadModel();

private:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    //! How many images to work on at the same time.
    // TODO: Make this constexpr and use std::array instead of std::vector to contain all data.
    const int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t m_currentFrame = 0;

    Window m_window{WIDTH, HEIGHT, "Vulkan"};
    Device m_device{m_window};

	VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
    std::vector<VkImageView> m_swapChainImageViews;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;

    VkRenderPass m_renderPass;
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

    // Sync objects
    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;

    // Image
    VkImage m_textureImage;
    VkDeviceMemory m_textureImageMemory;
    VkImageView m_textureImageView;

    // Texture Sampler
    VkSampler m_textureSampler;

    // Depth Image
    VkImage m_depthImage;
    VkDeviceMemory m_depthImageMemory;
    VkImageView m_depthImageView;

	// Module
	const std::string m_pathModel = "../resources/models/viking_room.obj";
	const std::string m_pathTexture = "../resources/textures/viking_room.png";
	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;

public:
    bool m_framebufferResized = false;
};