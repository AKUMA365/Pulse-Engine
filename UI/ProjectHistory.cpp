#include "ProjectHistory.h"
#include "../system/SystemInfo.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>

namespace fs = std::filesystem;

QString ProjectHistory::GetHistoryFilePath() {
    return QString::fromStdString((SystemInfo::GetConfigPath() / "projects.json").string());
}

void ProjectHistory::AddProject(const std::string& name, const fs::path& path) {
    auto projects = GetProjects();

    QString currentDate = QDateTime::currentDateTime().toString("dd.MM.yy HH:mm");

    std::erase_if(projects, [&](const ProjectMetadata& p) {
        return fs::equivalent(p.path, path);
    });

    ProjectMetadata newMeta;
    newMeta.name = name;
    newMeta.path = path;
    newMeta.lastUpdate = currentDate.toStdString();
    
    projects.insert(projects.begin(), newMeta);

    Save(projects);
}

std::vector<ProjectMetadata> ProjectHistory::GetProjects() {
    std::vector<ProjectMetadata> result;
    QFile file(GetHistoryFilePath());

    if (!file.open(QIODevice::ReadOnly)) return result;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = doc.object();
    QJsonArray array = root["all_projects"].toArray();

    for (const auto& item : array) {
        QJsonObject obj = item.toObject();
        
        fs::path p(obj["path"].toString().toStdString());

        if (fs::exists(p)) {
            ProjectMetadata meta;
            meta.name = obj["name"].toString().toStdString();
            meta.lastUpdate = obj["last_update"].toString().toStdString();
            meta.path = p;
            result.push_back(meta);
        }
    }
    return result;
}

void ProjectHistory::Save(const std::vector<ProjectMetadata>& projects) {
    QJsonArray array;

    for (const auto& meta : projects) {
        QJsonObject obj;
        obj["name"] = QString::fromStdString(meta.name);
        obj["path"] = QString::fromStdString(meta.path.generic_string());
        obj["last_update"] = QString::fromStdString(meta.lastUpdate);
        
        array.append(obj);
    }

    QJsonObject root;
    root["all_projects"] = array;

    QFile file(GetHistoryFilePath());
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson());
    }
}