#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <optional>

// Presentation queue family could differ from graphics queue family. (p. 75)
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;       // min/max number images, min/max size image, etc
    std::vector<VkSurfaceFormatKHR> formats;     // pixel format, color space, etc
    std::vector<VkPresentModeKHR> presentModes;  // presentation modes
};

// TODO: Add chapters
//  - Callback messages (52ff)
//  - Debugging instance creation and destruction (57ff)
class TriangleApp {
public:
    TriangleApp();
    ~TriangleApp();

    void run();

private:
    void initWindow();

    void initVulkan();
    void createVulkanInstance();
    bool checkValidationLayerSupport();
    void createSurface();
    
    void pickPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    
    void createLogicalDevice();

    void createSwapChain();
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    
    void createImageViews();
    void createGraphicsPipeline();
    VkShaderModule createShadersModule(const std::vector<char>& code);

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

    //! The set of extensions we reqiure for our task.
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    GLFWwindow* m_window;
    VkInstance m_instance;
    VkSurfaceKHR m_surface;

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device;  // Logical device
    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;

    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
    std::vector<VkImageView> m_swapChainImageViews;
};