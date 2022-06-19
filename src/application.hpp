#pragma once

#include <memory>

#include "core/window.hpp"
#include "core/device.hpp"
#include "core/renderer.hpp"
#include "core/descriptor.hpp"
#include "core/model.hpp"

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
	Model m_modelViking{m_device, "../resources/models/viking_room.obj","../resources/textures/viking_room.png"};
};
