#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

class Window {
public:
	Window(int width, int height, const std::string& name);
	~Window();

	bool shouldClose();
	void update();
	void stopWhileMinimized();

	// Getters
	VkExtent2D extent();
	GLFWwindow* handle();

	//! Check if window resized since last time calling this function.
	bool resized();

private:
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

private:
	int m_width;
	int m_height;
	std::string m_name;

	GLFWwindow* m_window;

public:
	bool m_framebufferResized = false;
};
