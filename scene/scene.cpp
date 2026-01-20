#include "scene.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <cmath>
#include <fstream>
#include <sstream>

entt::entity scene::CreateEntity(const std::string& name)
{
    return CreateEntityWithTexture(name, "ground.png");
}

entt::entity scene::CreateEntityWithTexture(const std::string& name, const std::string& texturePath)
{
    entt::entity entity = m_Registry.create();
    m_Registry.emplace<TransformComponent>(entity);

    auto& tag = m_Registry.emplace<TagComponent>(entity);
    tag.Tag = name.empty() ? "Entity" : name;

    auto& spriteComp = m_Registry.emplace<SpriteComponent>(entity);

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

void scene::update(float dt)
{
    if (!m_IsRunning) return;

    auto view = m_Registry.view<TransformComponent, NativeScriptComponent>();
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& script = view.get<NativeScriptComponent>(entity);

        if (script.FilePath.empty()) continue;

        std::ifstream file(script.FilePath);
        std::string line;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string command;
            iss >> command;

            if (command == "MOVE_X") {
                float speed;
                if (iss >> speed) transform.Position.x += speed * dt;
            }
            else if (command == "MOVE_Y") {
                float speed;
                if (iss >> speed) transform.Position.y += speed * dt;
            }
            else if (command == "ROTATE") {
                float speed;
                if (iss >> speed) transform.Rotation += speed * dt;
            }
            else if (command == "SCALE") {
                float factor;
                if (iss >> factor) {
                    transform.Scale.x += factor * dt;
                    transform.Scale.y += factor * dt;
                }
            }
        }
    }
}

void scene::StartRuntime()
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
        if (m_Registry.all_of<NativeScriptComponent>(entity)) {
            auto& script = m_Registry.get<NativeScriptComponent>(entity);
            entityObj["ScriptPath"] = QString::fromStdString(script.FilePath);
        }
        entitiesArray.append(entityObj);
    }
    QJsonObject root;
    root["Entities"] = entitiesArray;
    m_BackupScene = QJsonDocument(root).toJson().toStdString();

    m_IsRunning = true;
}

void scene::StopRuntime()
{
    m_IsRunning = false;

    if (m_BackupScene.empty()) return;

    m_Registry.clear();
    QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(m_BackupScene));
    QJsonObject root = doc.object();
    QJsonArray entitiesArray = root["Entities"].toArray();

    for (const auto& entityRef : entitiesArray) {
        QJsonObject entityObj = entityRef.toObject();

        std::string name = entityObj["Tag"].toString().toStdString();
        std::string texture = "ground.png";
        if (entityObj.contains("Texture")) texture = entityObj["Texture"].toString().toStdString();

        entt::entity entity = CreateEntityWithTexture(name, texture);

        auto& transform = m_Registry.get<TransformComponent>(entity);
        QJsonObject transformObj = entityObj["Transform"].toObject();
        transform.Position.x = (float)transformObj["PosX"].toDouble();
        transform.Position.y = (float)transformObj["PosY"].toDouble();
        transform.Rotation = (float)transformObj["Rot"].toDouble();
        transform.Scale.x = (float)transformObj["ScaleX"].toDouble();
        transform.Scale.y = (float)transformObj["ScaleY"].toDouble();

        if (entityObj.contains("ScriptPath")) {
            auto& script = m_Registry.emplace<NativeScriptComponent>(entity);
            script.FilePath = entityObj["ScriptPath"].toString().toStdString();
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

        if (m_Registry.all_of<NativeScriptComponent>(entity)) {
            auto& script = m_Registry.get<NativeScriptComponent>(entity);
            entityObj["ScriptPath"] = QString::fromStdString(script.FilePath);
        }

        entitiesArray.append(entityObj);
    }

    QJsonObject rootObject;
    rootObject["Scene"] = "Untitled";
    rootObject["Entities"] = entitiesArray;

    QFile saveFile(QString::fromStdString(filepath));
    if (saveFile.open(QIODevice::WriteOnly)) {
        saveFile.write(QJsonDocument(rootObject).toJson());
    }
}

void scene::LoadScene(const std::string& filepath)
{
    QFile loadFile(QString::fromStdString(filepath));
    if (!loadFile.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject root = loadDoc.object();

    m_Registry.clear();

    QJsonArray entitiesArray = root["Entities"].toArray();

    for (const auto& entityRef : entitiesArray) {
        QJsonObject entityObj = entityRef.toObject();

        std::string name = entityObj["Tag"].toString().toStdString();
        std::string texture = "ground.png";
        if (entityObj.contains("Texture")) texture = entityObj["Texture"].toString().toStdString();

        entt::entity entity = CreateEntityWithTexture(name, texture);

        auto& transform = m_Registry.get<TransformComponent>(entity);
        QJsonObject transformObj = entityObj["Transform"].toObject();
        transform.Position.x = (float)transformObj["PosX"].toDouble();
        transform.Position.y = (float)transformObj["PosY"].toDouble();
        transform.Rotation = (float)transformObj["Rot"].toDouble();
        transform.Scale.x = (float)transformObj["ScaleX"].toDouble();
        transform.Scale.y = (float)transformObj["ScaleY"].toDouble();

        if (entityObj.contains("ScriptPath")) {
            auto& script = m_Registry.emplace<NativeScriptComponent>(entity);
            script.FilePath = entityObj["ScriptPath"].toString().toStdString();
        }
    }
}