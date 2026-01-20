#include "scene.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

entt::entity scene::CreateEntity(const std::string& name)
{
    entt::entity entity = m_Registry.create();
    m_Registry.emplace<TransformComponent>(entity);

    auto& tag = m_Registry.emplace<TagComponent>(entity);
    tag.Tag = name.empty() ? "Entity" : name;

    auto& spriteComp = m_Registry.emplace<SpriteComponent>(entity);

    std::string textureName = "ground.png";

    if (m_Textures.find(textureName) == m_Textures.end()) {
        sf::Texture tex;
        if (!tex.loadFromFile(textureName)) {
            sf::Image whiteImage;
            whiteImage.resize(sf::Vector2u(100, 100), sf::Color::White);
            if(tex.loadFromImage(whiteImage)) {}
        }
        m_Textures[textureName] = tex;
    }

    spriteComp.Sprite.emplace(m_Textures[textureName]);
    spriteComp.TexturePath = textureName;
    spriteComp.TextureLoaded = true;

    sf::FloatRect bounds = spriteComp.Sprite->getLocalBounds();
    spriteComp.Sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

    return entity;
}

void scene::DestroyEntity(entt::entity entity)
{
    m_Registry.destroy(entity);
}

void scene::render(sf::RenderWindow& window)
{
    auto view = m_Registry.view<TransformComponent, SpriteComponent>();
    for (auto entity : view)
    {
        auto [transform, sprite] = view.get<TransformComponent, SpriteComponent>(entity);

        if (sprite.Sprite.has_value()) {
            sprite.Sprite->setPosition(transform.Position);
            sprite.Sprite->setRotation(sf::degrees(transform.Rotation));
            sprite.Sprite->setScale(transform.Scale);
            window.draw(*sprite.Sprite);
        }
    }
}

void scene::SaveScene(const std::string& filepath)
{
    QJsonArray entitiesArray;
    auto view = m_Registry.view<TagComponent, TransformComponent>();

    for (auto entity : view) {
        QJsonObject entityObj;

        auto& tag = view.get<TagComponent>(entity);
        auto& transform = view.get<TransformComponent>(entity);

        entityObj["Tag"] = QString::fromStdString(tag.Tag);

        QJsonObject transformObj;
        transformObj["PosX"] = transform.Position.x;
        transformObj["PosY"] = transform.Position.y;
        transformObj["Rot"] = transform.Rotation;
        transformObj["ScaleX"] = transform.Scale.x;
        transformObj["ScaleY"] = transform.Scale.y;
        entityObj["Transform"] = transformObj;

        if (m_Registry.all_of<SpriteComponent>(entity)) {
            auto& sprite = m_Registry.get<SpriteComponent>(entity);
            entityObj["Texture"] = QString::fromStdString(sprite.TexturePath);
        }

        entitiesArray.append(entityObj);
    }

    QJsonObject rootObject;
    rootObject["Scene"] = "Untitled";
    rootObject["Entities"] = entitiesArray;

    QFile saveFile(QString::fromStdString(filepath));
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Could not open file for save");
        return;
    }

    QJsonDocument saveDoc(rootObject);
    saveFile.write(saveDoc.toJson());
}

void scene::LoadScene(const std::string& filepath)
{
    QFile loadFile(QString::fromStdString(filepath));
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Could not open file for load");
        return;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject root = loadDoc.object();

    m_Registry.clear();

    QJsonArray entitiesArray = root["Entities"].toArray();

    for (const auto& entityRef : entitiesArray) {
        QJsonObject entityObj = entityRef.toObject();

        entt::entity entity = m_Registry.create();

        auto& tag = m_Registry.emplace<TagComponent>(entity);
        tag.Tag = entityObj["Tag"].toString().toStdString();

        auto& transform = m_Registry.emplace<TransformComponent>(entity);
        QJsonObject transformObj = entityObj["Transform"].toObject();
        transform.Position.x = (float)transformObj["PosX"].toDouble();
        transform.Position.y = (float)transformObj["PosY"].toDouble();
        transform.Rotation = (float)transformObj["Rot"].toDouble();
        transform.Scale.x = (float)transformObj["ScaleX"].toDouble();
        transform.Scale.y = (float)transformObj["ScaleY"].toDouble();

        if (entityObj.contains("Texture")) {
            auto& spriteComp = m_Registry.emplace<SpriteComponent>(entity);
            std::string texturePath = entityObj["Texture"].toString().toStdString();

            if (m_Textures.find(texturePath) == m_Textures.end()) {
                sf::Texture tex;
                if (!tex.loadFromFile(texturePath)) {
                    sf::Image whiteImage;
                    whiteImage.resize(sf::Vector2u(100, 100), sf::Color::White);
                    if(tex.loadFromImage(whiteImage)) {}
                }
                m_Textures[texturePath] = tex;
            }

            spriteComp.Sprite.emplace(m_Textures[texturePath]);
            spriteComp.TexturePath = texturePath;
            spriteComp.TextureLoaded = true;

            sf::FloatRect bounds = spriteComp.Sprite->getLocalBounds();
            spriteComp.Sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        }
    }
}