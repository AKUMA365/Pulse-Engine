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
    void DestroyEntity(entt::entity entity);

    void render(sf::RenderWindow& window);

    void SaveScene(const std::string& filepath);
    void LoadScene(const std::string& filepath);

    entt::registry& GetRegistry() { return m_Registry; }

private:
    entt::registry m_Registry;
    std::map<std::string, sf::Texture> m_Textures;
};

#endif