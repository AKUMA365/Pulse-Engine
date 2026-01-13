//
// Created by Dima Semchenko on 13.01.2026.
//

#include "scene.h"

#include <fmt/base.h>

void scene::init()
{
    if (!m_texture.loadFromFile("ground.png")) {
        fmt::print("Error loading ground.png\n");
    }
    m_sprite.setTexture(m_texture);

    if (!m_font.openFromFile("arial.ttf")) {
        fmt::print("Unable to load font\n");
    }
    m_text.setFont(m_font);
    m_text.setString("Hello PulseEngine");
    m_text.setCharacterSize(50);
    m_text.setFillColor(sf::Color::White);
    m_text.setPosition({10, 10});
}

void scene::render(sf::RenderWindow& window)
{
    window.draw(m_sprite);
    window.draw(m_text);
}