#pragma once

#include <vector>

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;       // min/max number images, min/max size image, etc
	std::vector<VkSurfaceFormatKHR> formats;     // pixel format, color space, etc
	std::vector<VkPresentModeKHR> presentModes;  // presentation modes
};
