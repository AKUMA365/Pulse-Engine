//
// Created by Dima Semchenko on 05.01.2026.
//

#include "scene.h"

#include <fmt/base.h>

#if defined(__APPLE__)
    #define VK_USE_PLATFORM_METAL_EXT // Активирует расширения для macOS/iOS
    #include <vulkan/vulkan.h>
    #include <vulkan/vulkan_metal.h>
#else
    #include <vulkan/vulkan.h>
#endif
#include <stdexcept>
#include <vector>

uint32_t Scene::ExtensionCount()
{
    uint32_t extensionCount{};

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    return extensionCount;
}

VkInstance Scene::CreateInstance(
    const char* appName,
    uint32_t appVersion)
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = appName;
    appInfo.applicationVersion = appVersion;
    appInfo.pEngineName = "PulseEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    // Расширения для кроссплатформенности
    std::vector<const char*> extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME
    };

#if defined(_WIN32)
    extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(__APPLE__)
    extensions.push_back(VK_EXT_METAL_SURFACE_EXTENSION_NAME);
#elif defined(__linux__)
    extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

    VkInstance instance;
    VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);

    if(result != VK_SUCCESS)
    {
        fmt::print(stderr, "Vulkan Error: {}\n", (int)result);
        throw std::runtime_error("Failed to create Vulkan instance!");
    }

    return instance;
};
