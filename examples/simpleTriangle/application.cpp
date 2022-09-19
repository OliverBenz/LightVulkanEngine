#include "application.hpp"
#include "triangle.hpp"
#include "renderSystem.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include <vector>

void Application::run() {
	Triangle triangle(m_device);
	RenderSystem renderSystem{m_device, m_renderer.swapchainRenderPass()};

	// Render loop
	while(!m_window.shouldClose()) {
		glfwPollEvents();

		// Start rendering
		VkCommandBuffer commandBuffer = m_renderer.beginFrame();
		if(commandBuffer) {
			m_renderer.beginSwapchainRenderPass(commandBuffer);

			// Rendering ouf stuff
			renderSystem.staticDraw(commandBuffer, triangle);

			// End rendering
			m_renderer.endSwapchainRenderPass(commandBuffer);
			m_renderer.endFrame();
		}
	}

	vkDeviceWaitIdle(m_device.device());
}
