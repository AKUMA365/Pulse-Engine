//
// Created by Dima Semchenko on 13.01.2026.
//

#include "scene.h"
#include <fmt/base.h>

scene::scene() : m_sprite(m_texture)
{
}

void scene::init()
{
    if (!m_texture.loadFromFile("ground.png")) {
        fmt::print("Error loading ground.png\n");
    }
    m_sprite.setTexture(m_texture);
}

void scene::render(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}