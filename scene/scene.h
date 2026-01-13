//
// Created by Dima Semchenko on 13.01.2026.
//

#ifndef PULSEENGINE_SCENE_H
#define PULSEENGINE_SCENE_H

#pragma once
#include <SFML/Graphics.hpp>

class scene
{
public:
    scene();
    void init();
    void render(sf::RenderWindow& window);
private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
};


#endif //PULSEENGINE_SCENE_H