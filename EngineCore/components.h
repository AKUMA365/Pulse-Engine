#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>
#include <optional>
#include <SFML/Graphics.hpp>

struct TagComponent {
    std::string Tag;
};

struct TransformComponent {
    sf::Vector2f Position = {0.0f, 0.0f};
    sf::Vector2f Scale = {1.0f, 1.0f};
    float Rotation = 0.0f;
};

struct SpriteComponent {
    std::optional<sf::Sprite> Sprite;
    std::string TexturePath;
    bool TextureLoaded = false;

    SpriteComponent() = default;
    explicit SpriteComponent(const sf::Texture& texture)
        : Sprite(texture), TextureLoaded(true) {}
};

struct NativeScriptComponent {
    std::string FilePath;
};

#endif