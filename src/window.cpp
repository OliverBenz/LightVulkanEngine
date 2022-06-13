#include "window.hpp"

#include <stdexcept>

Window::Window(int width, int height, const std::string& name) : m_width(width), m_height(height), m_name(name) {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);  // Do not create OpenGL context

	m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(m_window, this);  // Give glfw pointer to this class so we can access it in callback functions.
	glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
}

GLFWwindow* Window::handle() {
	return m_window;
}

VkExtent2D Window::extent() {
	return {static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height)};
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	Window* win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	win->m_framebufferResized = true;
	win->m_width = width;
	win->m_height = height;
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(m_window);
}

void Window::update() {
	glfwPollEvents();
}

void Window::stopWhileMinimized() {
	int width = 0, height = 0;
	glfwGetFramebufferSize(m_window, &width, &height);

	while(width == 0 || height == 0) {
		glfwGetFramebufferSize(m_window, &width, &height);
		glfwWaitEvents();
	}
}

bool Window::resized() {
	const bool val = m_framebufferResized;
	m_framebufferResized = false;

	return val;
}

Window::~Window() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}
