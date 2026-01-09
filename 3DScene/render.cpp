//
// Created by Dima Semchenko on 06.01.2026.
//

#include "render.h"

#pragma once
#include <QVulkanDeviceFunctions>
#include <QDebug>
#include <QVulkanInstance>

QVulkanWindowRenderer* VulkanWindow::createRenderer()
{
    return new VulkanRenderer(this);
}

VulkanRenderer::VulkanRenderer(QVulkanWindow* w)
    : m_window(w)
{
}

void VulkanRenderer::initResources()
{
    qDebug("initializing resources");

    m_devFuncs = m_window->vulkanInstance()->deviceFunctions(m_window->device());

    uint32_t imgCount = m_window->swapChainImageCount();
    m_renderSemaphores.resize(imgCount);

    VkSemaphoreCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    for (auto& s : m_renderSemaphores)
        m_devFuncs->vkCreateSemaphore(m_window->device(), &info, nullptr, &s);

    m_devFuncs = m_window->vulkanInstance()->deviceFunctions(m_window->device());
    // TODO: Создать pipeline для треугольника (упрощённо)
}

void VulkanRenderer::initSwapChainResources()
{
    //TODO: Here will be resources dependent on swapchain: framebuffers, renderpass, shaders
}

void VulkanRenderer::releaseResources()
{
    for (auto& s : m_renderSemaphores)
        m_devFuncs->vkDestroySemaphore(m_window->device(), s, nullptr);
    m_renderSemaphores.clear();
}

void VulkanRenderer::releaseSwapChainResources()
{
    //TODO: Make a swapchain
}

void VulkanRenderer::startNextFrame()
{
    m_green += 0.005f;
    if (m_green > 1.0f) m_green = 0.0f;

    VkClearColorValue clearColor = {{0.0f, m_green, 0.0f, 1.0f}};
    VkClearDepthStencilValue clearDS = {1.0f, 0};
    VkClearValue clearValues[2]{};
    clearValues[0].color = clearColor;
    clearValues[1].depthStencil = clearDS;

    VkRenderPassBeginInfo rpBeginInfo{};
    rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpBeginInfo.renderPass = m_window->defaultRenderPass();
    rpBeginInfo.framebuffer = m_window->currentFramebuffer();

    QSize sz = m_window->swapChainImageSize();
    rpBeginInfo.renderArea.extent.width = sz.width();
    rpBeginInfo.renderArea.extent.height = sz.height();

    rpBeginInfo.clearValueCount = 2;
    rpBeginInfo.pClearValues = clearValues;

    VkCommandBuffer cmdBuf = m_window->currentCommandBuffer();

    m_devFuncs->vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    m_devFuncs->vkCmdEndRenderPass(cmdBuf);

    m_window->frameReady();
    m_window->requestUpdate();
}