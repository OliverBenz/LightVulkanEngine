#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>

#include "device.hpp"
#include "window.hpp"

class Swapchain {
public:
	Swapchain(Window& window, Device& device);
	~Swapchain();

	VkExtent2D extent() const;
	uint32_t currentFrame() const;
	VkRenderPass renderPass() const;
	VkFramebuffer frameBuffer(uint32_t imageIndex) const;

	//! Get the next image and write image index to variable.
	VkResult getNextImage(uint32_t& imageIndex);

	// TODO: Move this function to renderer?
	//! Submit a command buffer to the graphics queue.
	VkResult submitCommandBuffer(VkCommandBuffer& commandBuffer, uint32_t& imageIndex);

private:
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createDepthResources();
	void createFramebuffers();
	void createSyncObjects();

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

	// Depth Image
	VkFormat findDepthFormat() const;
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
					 VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

public:
	//! How many images to work on at the same time.
	static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

private:
	uint32_t m_currentFrame = 0;

	// Owned by application
	Window& m_window;
	Device& m_device;

	VkSwapchainKHR m_swapChain;
	VkRenderPass m_renderPass;

	VkFormat m_imageFormat;
	VkExtent2D m_extent;

	std::vector<VkImage> m_images;
	std::vector<VkImageView> m_imageViews;
	std::vector<VkFramebuffer> m_framebuffers;

	// Depth Image
	VkImage m_depthImage;
	VkDeviceMemory m_depthImageMemory;
	VkImageView m_depthImageView;

	// Sync objects
	std::vector<VkSemaphore> m_imageAvailableSemaphores;
	std::vector<VkSemaphore> m_renderFinishedSemaphores;
	std::vector<VkFence> m_inFlightFences;
};
