// Pulse Engine
// File: ${NAME}
// Created on ${DATE}
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#ifndef PULSEENGINE_LOGGER_H
#define PULSEENGINE_LOGGER_H

#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <fmt/core.h>

class Log {
public:
    static void Init();

    static std::shared_ptr<spdlog::logger>& GetCoreLogger();
    static std::shared_ptr<spdlog::logger>& GetFileLogger();

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_FileLogger;
};

#endif //PULSEENGINE_LOGGER_H
