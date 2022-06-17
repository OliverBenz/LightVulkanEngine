#pragma once

#include "object.hpp"
#include "device.hpp"
#include "buffer.hpp"

#include <string>
#include <vector>
#include <memory>

class Model : public Object {
public:
	Model(Device& device, const std::string pathModel, const std::string pathTexture);
	~Model();

	void bind(VkCommandBuffer commandBuffer);  //! Bind vertices and indices to command buffer.
	void draw(VkCommandBuffer commandBuffer);  //! Add draw command to command buffer.

	VkSampler sampler() const;
	VkImageView imageView() const;

private:
	//! Load model files and write to GPU buffers.
	void loadModel();

	void createVertexBuffer(std::vector<Vertex>& vertices);
	void createIndexBuffer(std::vector<uint32_t>& indices);

	// Texture TODO: Better design.. Should this be in here? Some functions are duplicated.
	void createTextureSampler();
	void createTextureImage();
	void createTextureImageView();
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
	                 VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

private:
	// Owned by application
	Device& m_device;

	std::string m_pathModel;
	std::string m_pathTexture;

	std::unique_ptr<Buffer> m_vertexBuffer;
	std::unique_ptr<Buffer> m_indexBuffer;
	bool m_hasIndexBuffer = false;  //! Vertices can also be drawn non indexed.

	VkSampler m_textureSampler;

	VkImage m_textureImage;
	VkDeviceMemory m_textureImageMemory;
	VkImageView m_textureImageView;
};
