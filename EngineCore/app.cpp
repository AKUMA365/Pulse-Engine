// Pulse Engine
// File: ${NAME}
// Created on ${DATE}
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.
#define CORE_TRACE

#include "app.h"
#include "logginer.h"
#include "../log/logs.h"
#include <memory>
#include <QVulkanInstance>
#include <QGuiApplication>
#include <QByteArrayList>
#include <QVersionNumber>

#include "../3DScene/render.h"
#include "../3DScene/scene.h"
#include "../system/SystemInfo.h"

void app::Run() {
    while (m_IsRunning) {
    }
}

void app::Init(int argc, char** argv) {
    Log::Init();
    m_Logs = std::make_unique<Logs>();
    m_Logs->PE_INFO("Logs Initialized");

    SystemInfo SI;
    SI.Platform();
    m_Logs->PE_INFO("Platform system info detected");
    auto data = SI.GetData();
    SI.Print();

    QGuiApplication qtApp(argc, argv);

    QVulkanInstance inst;

    QByteArrayList instanceExtensions = inst.extensions();
    instanceExtensions.append("VK_KHR_get_physical_device_properties2");
    instanceExtensions.append("VK_KHR_portability_enumeration");
    instanceExtensions.append("VK_EXT_metal_surface");
    inst.setExtensions(instanceExtensions);

    inst.setApiVersion(QVersionNumber(1, 2));

    if (!inst.create()) {
        m_Logs->PE_ERROR("Failed to create Vulkan instance!");
        return;
    }
    m_Logs->PE_INFO("Vulkan Instance Created");

    VulkanWindow* w = new VulkanWindow();
    w->setVulkanInstance(&inst);

    QSurfaceFormat format = w->format();
    format.setAlphaBufferSize(0);
    w->setFormat(format);

    QByteArrayList deviceExtensions;
    deviceExtensions.append("VK_KHR_portability_subset");
    w->setDeviceExtensions(deviceExtensions);

    w->resize(1024, 768);
    w->setTitle("PulseEngine - Vulkan");
    w->show();

    if (w->isValid()) {
        w->requestUpdate();
    }

    m_Logs->PE_INFO("Window shown. Starting Event Loop.");

    qtApp.exec();

    delete w;
    m_Logs->PE_INFO("Engine shutting down.");
}