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

class Application {
public:
    Application();
    ~Application();

    void run();

private:
    void createUniformBuffers();
    void createDescriptorSets();

    void updateUniformBuffer(uint32_t currentImage, glm::vec3 offset);

private:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    Window m_window{WIDTH, HEIGHT, "Vulkan"};
    Device m_device{m_window};

	std::unique_ptr<DescriptorPool> m_descriptorPool;

	// TODO: These don't have to be class members; can be local.
	std::unique_ptr<VkDescriptorSetLayout> m_descriptorSetLayout;
    std::vector<VkDescriptorSet> m_descriptorSets;

	// TODO: Remove pipeline from this and use pipeline in indivitual systems.
	Renderer m_renderer{m_device, m_window, m_descriptorSetLayout};

	// TODO: Move this somewhere
    std::vector<VkBuffer> m_uniformBuffers;
    std::vector<VkDeviceMemory> m_uniformBuffersMemory;

	// Model
	Model m_modelViking{m_device, "../resources/models/viking_room.obj","../resources/textures/viking_room.png"};
};
