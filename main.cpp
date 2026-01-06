// Pulse Engine
// File: ${NAME}
// Created on ${DATE}
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#include "main.h"

#include <spdlog/fmt/fmt.h>

#include "EngineCore/app.h"
#include "3DScene/scene.h"

int main(int argc, char** argv) {
    app app;

    app.Init();
    app.Run();

    Scene scene;

    return 0;
}