// Pulse Engine
// File: ${NAME}
// Created on ${DATE}
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#include "main.h"

#include <QGuiApplication>
#include <spdlog/fmt/fmt.h>
#include <QTimer>

#include "3DScene/render.h"
#include "EngineCore/app.h"
#include "3DScene/scene.h"


int main(int argc, char** argv) {
    app ap;

    ap.Init(argc, argv);


    QGuiApplication app(argc, argv);

    QVulkanInstance inst;
    inst.setLayers({ "VK_LAYER_KHRONOS_validation" }); // опционально
    if (!inst.create()) {
        qFatal("Failed to create Vulkan instance");
    }

    VulkanWindow window;
    window.setVulkanInstance(&inst);

    QTimer::singleShot(16, [&]() { window.requestUpdate(); });
    window.resize(800, 600);
    window.show();

    ap.Run();

    Scene scene;

    return app.exec();
}