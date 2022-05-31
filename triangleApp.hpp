#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class TriangleApp {
public:
    TriangleApp();
    ~TriangleApp();

    void run();

private:
    void initWindow();
    void initVulkan();
    void createVulkanInstance();
    void mainLoop();

    bool checkValidationLayerSupport();

private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    // Turn off validation layers on release builds for better performance.
    const std::vector<const char*> m_validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
#ifdef NDEBUG
    const bool m_enableValidationLayers = false;
#else
    const bool m_enableValidationLayers = true;
#endif

    GLFWwindow* m_window;
    VkInstance m_instance;
};