#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>

#include "device.hpp"

class Swapchain {
public:
	Swapchain(Device& device);
	~Swapchain();

private:
	void createSwapChain();
	void cleanupSwapChain();
	void createImageViews();
	void recreateSwapChain();
	void createRenderPass();
	void createDepthResources();
	void createFramebuffers();
	void createSyncObjects();

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	// Depth Image
	VkFormat findDepthFormat();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

private:
	//! How many images to work on at the same time.
	static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

	uint32_t m_currentFrame = 0;

	// Owned by application
	Device& m_device;

	VkSwapchainKHR m_swapChain;
	VkRenderPass m_renderPass;

	VkFormat m_swapChainImageFormat;
	VkExtent2D m_swapChainExtent;

	// TODO: Refactor
	std::vector<VkImage> m_swapChainImages;
	std::vector<VkImageView> m_swapChainImageViews;
	std::vector<VkFramebuffer> m_swapChainFramebuffers;

	// Depth Image
	VkImage m_depthImage;
	VkDeviceMemory m_depthImageMemory;
	VkImageView m_depthImageView;

	// Sync objects
	std::vector<VkSemaphore> m_imageAvailableSemaphores;
	std::vector<VkSemaphore> m_renderFinishedSemaphores;
	std::vector<VkFence> m_inFlightFences;
};
