#pragma once

#include <memory>

#include "lwEngine/window.hpp"
#include "lwEngine/device.hpp"
#include "lwEngine/renderer.hpp"
#include "lwEngine/descriptor.hpp"
#include "lwEngine/model.hpp"

class Application {
public:
    void run();

private:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    Window m_window{WIDTH, HEIGHT, "Vulkan"};
    Device m_device{m_window};
	Renderer m_renderer{m_device, m_window};

	std::unique_ptr<DescriptorPool> m_descriptorPool;
	Model m_modelViking{m_device, RESOURCE_PATH_VIKING_MODEL, RESOURCE_PATH_VIKING_TEXTURE};
};
