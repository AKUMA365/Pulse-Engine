// Pulse Engine
// File: ${NAME}
// Created on ${DATE}
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#include "logginer.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include  "spdlog/sinks/stdout_color_sinks-inl.h"

std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_FileLogger;

void Log::Init()
{
    try {
        // First, we create a console logger to see errors if the file is not created.
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

        // Creating file synchronizations
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log/core.txt", true);

        // We combine them into one logger (optional) or create them separately
        s_CoreLogger = std::make_shared<spdlog::logger>("Pulse engine", spdlog::sinks_init_list{console_sink, file_sink});

        // Setting the level and pattern
        s_CoreLogger->set_level(spdlog::level::trace);
        s_CoreLogger->set_pattern("[%Y-%m-%d %H:%M:%S] [%n] [%l] %v");

        // Register it so spdlog knows about it
        spdlog::register_logger(s_CoreLogger);

    } catch (const spdlog::spdlog_ex& ex) {
        fmt::print(stderr, "Critical Error: Log initialization failed: {}\n", ex.what());
    }
}
std::shared_ptr<spdlog::logger>& Log::GetCoreLogger() { return s_CoreLogger; }
std::shared_ptr<spdlog::logger>& Log::GetFileLogger() { return s_FileLogger; }
