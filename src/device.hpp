#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include <optional>

#include "window.hpp"

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;       // min/max number images, min/max size image, etc
	std::vector<VkSurfaceFormatKHR> formats;     // pixel format, color space, etc
	std::vector<VkPresentModeKHR> presentModes;  // presentation modes
};

// Presentation queue family could differ from graphics queue family. (p. 75)
struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class Device {
public:
	Device(Window& window);
	~Device();

	VkDevice device() const;
	VkSurfaceKHR surface() const;
	VkPhysicalDevice physicalDevice() const;
	VkCommandPool commandPool() const;
	VkQueue graphicsQueue() const;
	VkQueue presentQueue() const;

	bool validationLayersEnabled() const;

	// TODO: Make this prettier (getQueueFamilyInfo function or something like that..)
	QueueFamilyIndices findQueueFamilies() const;
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
	SwapChainSupportDetails getSwapChainSupport(VkPhysicalDevice device) const;

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

private:
	void createVulkanInstance();
	void createSurface();

	void pickPhysicalDevice();
	void createLogicalDevice();
	void createCommandPool();

	bool isDeviceSuitable(VkPhysicalDevice device) const;
	bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;
	bool checkValidationLayerSupport() const;

private:
#ifdef NDEBUG
	const bool m_enableValidationLayers = false;
#else
	const bool m_enableValidationLayers = true;
#endif

	Window& m_window; // Not owned by this class

	VkInstance m_instance;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_device;

	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;

	VkCommandPool m_commandPool;
	VkSurfaceKHR m_surface;

	const std::vector<const char*> m_deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	const std::vector<const char*> m_validationLayers = {"VK_LAYER_KHRONOS_validation"};
};
