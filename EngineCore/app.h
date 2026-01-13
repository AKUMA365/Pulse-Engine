// Pulse Engine
// File: ${NAME}
// Created on ${DATE}
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#ifndef PULSEENGINE_CORE_H
#define PULSEENGINE_CORE_H
#include <memory>
#include <SFML/Graphics.hpp>
#include <optional>

#include "../scene/scene.h"
#include "../log/logs.h"

class app {
    public:
        void Run();
        void Stop();
        void Init(int argc, char** argv);
        void shutdown();
    private:
        bool m_IsRunning = false;
        std::unique_ptr<Logs> m_Logs;

        std::optional<sf::RenderWindow> m_Window;

        scene m_Scene;
};


#endif //PULSEENGINE_CORE_H