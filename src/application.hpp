#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <optional>
#include <memory>

#include "device.hpp"
#include "window.hpp"
#include "vertex.hpp"
#include "renderer.hpp"
#include "descriptor.hpp"
#include "renderSystem.hpp"

class Application {
public:
    void run();

private:
	//! Need buffer descriptors from render systems.
    void createDescriptorSets(RenderSystem& rotationSystem);

    void updateUniformBuffer(uint32_t currentImage, glm::vec3 offset);

private:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    Window m_window{WIDTH, HEIGHT, "Vulkan"};
    Device m_device{m_window};
	Renderer m_renderer{m_device, m_window};

	std::unique_ptr<DescriptorPool> m_descriptorPool;
	// TODO: These don't have to be class members; can be local.
	std::unique_ptr<DescriptorSetLayout> m_descriptorSetLayout;
    std::vector<VkDescriptorSet> m_descriptorSets;

	// Render objects
	Model m_modelViking{m_device, "../resources/models/viking_room.obj","../resources/textures/viking_room.png"};
};
