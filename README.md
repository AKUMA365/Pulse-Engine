# Pulse Engine

Deterministic C++ simulation engine designed for serious educational and research applications, with a focus on cardiac surgery simulations.

## Overview
PulseSim Engine is a data-oriented, deterministic simulation engine that allows precise modeling of physiological systems.
Its primary goal is to provide a safe environment for studying and experimenting with simulation scenarios without risk to real subjects.

## Key Features
- Deterministic simulation core with fixed timestep
- Data-oriented ECS architecture for high performance
- Replay and timeline system to review actions
- Scenario-driven procedures for medical training simulations
- Tool interaction layer (e.g., surgical instruments simulation)
- Source code available for study only (see License)

## Architecture
- **Core:** deterministic simulation loop
- **ECS:** data-oriented entity-component-system
- **Physics:** simplified soft-body and fluid dynamics
- **Physiology Model:** cardiovascular simulation (heart, blood flow, tissues)
- **Event System:** actions and scenario events
- **Replay / Timeline:** rewind and analyze procedure steps
- **Tool Interaction Layer:** instruments interact with simulation
- **Visualization:** optional 3D representation for demonstration

## License
Proprietary – source code may be downloaded and studied only. All rights reserved by the author.  
Contact: telegram-@DmitrySemchenko

## Requirements
- C++23 compatible compiler (GCC / Clang / MSVC)
- CMake 3.26+
- Dependencies: glm, Eigen3, spdlog, fmt, vulkan,qt, EnTT (can be installed via vcpkg)

## Build & Run
1. Clone the repository
2. Configure with CMake: `cmake -S . -B build`
3. Build: `cmake --build build`
4. Run the executable in `build/bin/`

## Contact
For inquiries, permission requests, or collaboration, contact Dima Semchenko via Telegram: [@DmitrySemchenko](https://t.me/DmitrySemchenko)
