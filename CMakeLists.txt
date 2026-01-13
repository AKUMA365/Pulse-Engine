cmake_minimum_required(VERSION 3.26)
project(PulseEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    if(EXISTS "/opt/homebrew")
        list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
    endif()
endif()

find_package(glm REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(EnTT REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Gui)
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(PulseEngine
        main.cpp
        EngineCore/app.cpp
        EngineCore/time.cpp
        EngineCore/main_loop.cpp
        EngineCore/logginer.cpp
        system/SystemInfo.cpp
        log/logs.cpp
        scene/scene.cpp
        scene/scene.h
)

target_link_libraries(PulseEngine
        PRIVATE
        glm::glm
        Eigen3::Eigen
        spdlog::spdlog
        fmt::fmt
        EnTT::EnTT
        Qt6::Core
        Qt6::Gui
        SFML::Graphics
        SFML::Window
        SFML::System
)

if(WIN32)
    add_custom_command(TARGET PulseEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:PulseEngine>
            $<TARGET_FILE_DIR:PulseEngine>
            COMMAND_EXPAND_LISTS
    )
endif()