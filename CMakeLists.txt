cmake_minimum_required(VERSION 3.26)
project(PulseEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_TOOLCHAIN_FILE "C:/Users/semch/CLionProjects/PulseEngine/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")

# Libraries via vcpkg
find_package(glm CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

# Vulkan SDK manually
set(Vulkan_INCLUDE_DIR "C:/VulkanSDK/1.4.335.0/Include")
set(Vulkan_LIBRARY "C:/VulkanSDK/1.4.335.0/Lib/vulkan-1.lib")
find_package(Vulkan REQUIRED)

add_executable(PulseEngine
        main.cpp
        main.h
        EngineCore/app.cpp
        EngineCore/app.h
        EngineCore/time.cpp
        EngineCore/time.h
        EngineCore/main_loop.cpp
        EngineCore/main_loop.h
        EngineCore/logginer.cpp
        EngineCore/logginer.h
)

target_link_libraries(PulseEngine
        PRIVATE
        glm::glm
        Eigen3::Eigen
        spdlog::spdlog
        fmt::fmt
        EnTT::EnTT
        Vulkan::Vulkan
)
