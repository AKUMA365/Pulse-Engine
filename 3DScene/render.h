//
// Created by Dima Semchenko on 06.01.2026.
//

#ifndef PULSEENGINE_RENDER_H
#define PULSEENGINE_RENDER_H

#pragma once

#include <QVulkanWindow>
#include <QVulkanDeviceFunctions>
#include <vulkan/vulkan.h>
#include <vector>

class VulkanRenderer : public QVulkanWindowRenderer
{
public:
    VulkanRenderer(QVulkanWindow* w);

    void initResources() override;
    void initSwapChainResources() override;
    void startNextFrame() override;
    void releaseResources() override;
    void releaseSwapChainResources() override;

private:
    QVulkanWindow* m_window;
    QVulkanDeviceFunctions* m_devFuncs = nullptr;

    // Pipeline для простого треугольника
    VkPipeline m_pipeline = VK_NULL_HANDLE;
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

    // Семафоры для swapchain
    std::vector<VkSemaphore> m_renderSemaphores;

    float m_green = 0.0f;
};

class VulkanWindow : public QVulkanWindow
{
public:
    QVulkanWindowRenderer *createRenderer() override;
};

#endif //PULSEENGINE_RENDER_H