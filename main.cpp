// Pulse Engine
// File: ${NAME}
// Created on ${DATE}
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#include "main.h"

#include "UI/mainwindow.h"
#include "EngineCore/logginer.h"
#include "log/logs.h"
#include "system/SystemInfo.h"

#include <QApplication>

#include "EngineCore/main_loop.h"
#include "UI/StartProject.h"

int main(int argc, char** argv) {
    Log::Init();
    auto appLogs = std::make_unique<Logs>();
    appLogs->PE_INFO("Logs Initialized (Qt Mode)");

    SystemInfo SI;
    SI.Platform();
    SI.Detect();
    SI.Print();

    QApplication app(argc, argv);

    main_loop eLoop;

    std::thread eThread([&eLoop]()
    {
        eLoop.Run();
    });



    StartProject window;
    window.show();

    int result = app.exec();

    eLoop.stop();

    if (eThread.joinable())
    {
        eThread.join();
    }

    return result;
}
