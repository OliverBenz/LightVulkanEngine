#pragma once

#include <vulkan/vulkan.hpp>

#include "window.hpp"
#include "queueFamilyIndices.hpp"
#include "swapChainSupportDetails.hpp"

class Device {
public:
	Device(Window& window);
	~Device();

	VkDevice device();
	VkSurfaceKHR surface();
	VkPhysicalDevice physicalDevice();
	VkCommandPool commandPool();
	VkQueue graphicsQueue();
	VkQueue presentQueue();

	bool validationLayersEnabled();

	// TODO: Make this prettier (getQueueFamilyInfo function or something like that..)
	QueueFamilyIndices findQueueFamilies();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	SwapChainSupportDetails getSwapChainSupport(VkPhysicalDevice device);

private:
	void createVulkanInstance();
	void createSurface();

	void pickPhysicalDevice();
	void createLogicalDevice();
	void createCommandPool();

	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkValidationLayerSupport();

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
