//
// Created by Dima Semchenko on 05.01.2026.
//

#ifndef PULSEENGINE_SCENE_H
#define PULSEENGINE_SCENE_H
#pragma once

#include "../log/logs.h"
#include "vulkan/vulkan.h"

class Scene
{
public:
    uint32_t ExtensionCount();
    static VkInstance CreateInstance(const char* appName, uint32_t appVersion);
private:
    VkInstance m_Instance = VK_NULL_HANDLE;
};


#endif //PULSEENGINE_SCENE_H