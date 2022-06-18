#pragma once

#include "vertex.hpp"

#include <vulkan/vulkan.hpp>
#include <vector>

class Object {
public:
	virtual void bind(VkCommandBuffer commandBuffer);
	virtual void draw(VkCommandBuffer commandBuffer);
};