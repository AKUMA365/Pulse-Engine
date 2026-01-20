# 🎮 Pulse Engine

![Status](https://img.shields.io/badge/Status-Alpha-orange) ![Language](https://img.shields.io/badge/Language-C%2B%2B23-blue) ![License](https://img.shields.io/badge/License-Proprietary-red)

**Pulse Engine** is a custom, high-performance game and simulation engine written in **C++23**.
It combines the power of **SFML** for rendering and **Qt6** for a robust editor interface, orchestrated by a modern **Entity Component System (ECS)** architecture using EnTT.

This project serves as a research platform for exploring game engine architecture, physics simulations, and advanced software design patterns.

---

## ✨ Key Features (Alpha)

### 🏗️ Architecture
* **ECS Core:** Built on `EnTT`, providing a cache-friendly and highly performant architecture for game objects.
* **Scene Management:** Full serialization system (Save/Load) using JSON. Scenes preserve entity states, transforms, and attached scripts.
* **Runtime/Editor Mode:** Seamless switching between "Edit Mode" and "Play Mode" to test game logic in real-time.

### 🖥️ The Editor
* **Project Browser:** A dedicated hub to manage, create, and open projects with automatic directory structure generation.
* **Hierarchy & Inspector:** View all entities in the scene and modify their components (Position, Scale, Rotation, Scripts) in real-time.
* **Asset Browser:** Integrated file explorer to view and manage project assets (textures, scripts).
* **Viewport:** Infinite grid rendering with Zoom and Pan controls (Alt+LMB or Middle Mouse) for easy scene navigation.
* **Script Editor:** Built-in text editor for creating and modifying `.pulse` behavior scripts directly within the engine.

### 📜 Scripting System (PulseScript)
* **Native Command Parser:** A lightweight, custom scripting language designed for immediate feedback.
* **Hot-Reloading:** Attach scripts to entities and see behaviors instantly when running the scene.

---

## 🚀 Getting Started

### Prerequisites
To build Pulse Engine, you need a C++23 compliant compiler and the following dependencies:
* **Compiler:** GCC 13+, Clang 16+, or MSVC 2022.
* **CMake:** Version 3.26+.
* **Qt6:** Core, Gui, Widgets modules.
* **SFML 3:** Graphics, Window, System modules.
* **Third-Party Libs:** `EnTT`, `spdlog`, `fmt`, `glm`, `Eigen3`.

### Building
```bash
git clone [https://github.com/AKUMA365/Pulse-Engine.git](https://github.com/AKUMA365/Pulse-Engine.git)
cd Pulse-Engine
mkdir build && cd build
cmake ..
make
```
# Run the executable: ./PulseEngine

📚 User Guide
1. Project Management

Upon launching Pulse Engine, you are greeted by the Project Browser.

    New Project: Select a template (Blank or 2D), name your project, and choose a location. The engine will create the necessary folder structure.

    Open Project: Select a recently used project or browse for an existing project folder.

2. Scene Editing

    Adding Objects:

        Right-click in the Hierarchy or use the "Add Entity" button.

        Primitive Cube: Adds a default test object.

        Custom Object: Opens a file dialog to select an image (.png, .jpg). The engine automatically copies the asset to your project folder and creates an entity with a Sprite Component.

    Navigation:

        Pan: Hold Middle Mouse Button OR Alt + Left Click and drag.

        Zoom: Use the Mouse Wheel.

    Manipulation: Select an object in the Hierarchy to view its properties in the Inspector. You can manually adjust Position, Scale, and Rotation.

3. Saving and Loading

    Save (Ctrl+S): Saves the current state of the scene (Entities, Components, Scripts) to scene.json in your project folder.

    Auto-Load: When opening a project, the engine automatically loads the last saved scene state.

📝 Scripting Documentation

Pulse Engine uses a custom, lightweight scripting format (.pulse). These are simple text files interpreted by the engine at runtime.
How to Create a Script

    In the Editor Menu, go to File -> New Script.

    Enter a name (e.g., enemy_movement). The engine adds the .pulse extension automatically.

    The built-in Script Editor will open.

Script Syntax

The logic is defined by commands written line-by-line.
Command	Argument	Description	Example
MOVE_X	float	Moves the entity along the X-axis.	MOVE_X 100
MOVE_Y	float	Moves the entity along the Y-axis.	MOVE_Y -50
ROTATE	float	Rotates the entity (degrees per second).	ROTATE 90
SCALE	float	Scales the entity uniformly.	SCALE 0.5
Example Script (fly_circle.pulse)
Plaintext

MOVE_X 50
ROTATE 180
SCALE 0.1

Attaching Scripts

    Select an Entity in the Hierarchy.

    In the Inspector, locate the "Script" row.

    Click ... to browse and select your .pulse file.

    Press Play in the toolbar to see the logic in action!

🗺 Roadmap

    [x] Project Setup & CMake configuration

    [x] Qt6 Editor Interface (Dark Theme)

    [x] SFML 3.0 Integration (Rendering)

    [x] EnTT ECS Implementation

    [x] Serialization (JSON Save/Load)

    [x] Native Scripting System (.pulse)

    [x] Project & Asset Management

    [ ] Physics System (Box2D Integration)

    [ ] Advanced Scripting (Python/Lua)

    [ ] 3D Rendering (Vulkan backend)

📄 License

Proprietary / Academic Evaluation. Copyright © 2025 Dima Semchenko. See LICENSE.txt for details.