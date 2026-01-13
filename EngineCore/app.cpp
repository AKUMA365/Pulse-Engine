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

#include "../system/SystemInfo.h"

void app::Init(int argc, char** argv) {
    Log::Init();
    m_Logs = std::make_unique<Logs>();
    m_Logs->PE_INFO("Logs Initialized");

    SystemInfo SI;
    SI.Platform();
    auto data = SI.GetData();
    SI.Print();

    m_Window.emplace(sf::VideoMode({600, 300}), "Pulse Engine");

    m_Window->setFramerateLimit(60);
    m_Logs->PE_INFO("Window Created");

    m_Scene.init();

    m_IsRunning = true;

}

void app::Run() {
    while (m_IsRunning && m_Window->isOpen()) {
        while (const std::optional event = m_Window->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                m_Window->close();
                m_IsRunning = false;
            }
        }

        m_Window->clear(sf::Color::Magenta);

        m_Scene.render(*m_Window);

        m_Window->display();
    }
}

void app::Stop() {
    m_IsRunning = false;
}

void app::shutdown() {
    if (m_Window) {
        m_Window->close();
    }
}