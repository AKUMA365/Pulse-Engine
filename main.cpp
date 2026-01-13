// Pulse Engine
// File: ${NAME}
// Created on ${DATE}
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#include "main.h"

#include "EngineCore/app.h"
#include "UI/mainwindow.h"

#include <QApplication>

int main(int argc, char** argv) {
    app PulseEngine;

    PulseEngine.Init(argc, argv);

    PulseEngine.Run();

    QApplication app(argc, argv);

    PulseEngineMainWindow window;
    window.show();

    return 0;
}