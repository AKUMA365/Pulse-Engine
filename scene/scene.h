#ifndef PULSEENGINE_SCENE_H
#define PULSEENGINE_SCENE_H

#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "../EngineCore/components.h"

class scene
{
public:
    scene() = default;

    entt::entity CreateEntity(const std::string& name = "Entity");
    entt::entity CreateEntityWithTexture(const std::string& name, const std::string& texturePath);
    void DestroyEntity(entt::entity entity);

    void render(sf::RenderWindow& window);
    void update(float dt);

    void SaveScene(const std::string& filepath);
    void LoadScene(const std::string& filepath);

    void StartRuntime();
    void StopRuntime();

    entt::registry& GetRegistry() { return m_Registry; }

private:
    entt::registry m_Registry;
    std::map<std::string, sf::Texture> m_Textures;
    bool m_IsRunning = false;
    std::string m_BackupScene;
};

#endif