//
// Created by Dima Semchenko on 05.01.2026.
//

#include "scene.h"

#include "QVulkanWindow"
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

