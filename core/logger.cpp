// Pulse Engine
// File: ${NAME}
// Created on ${DATE}
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#include "logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"

std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_FileLogger;

void Log::Init()
{
    try {

        // Инициализация логгеров
        s_CoreLogger = spdlog::basic_logger_mt("core", "log/core.txt");
        s_FileLogger = spdlog::rotating_logger_mt("file", "log/file.txt", 1024 * 1024 * 5, 3);

        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
        spdlog::flush_on(spdlog::level::info);

    } catch (const spdlog::spdlog_ex& ex) {
        fmt::print(stderr, "Log init failed: {}\n", ex.what());
    }
}

std::shared_ptr<spdlog::logger>& Log::GetCoreLogger() { return s_CoreLogger; }
std::shared_ptr<spdlog::logger>& Log::GetFileLogger() { return s_FileLogger; }
