#ifndef PROJECTHISTORY_H
#define PROJECTHISTORY_H

#include <QString>
#include <vector>
#include <filesystem>

struct ProjectMetadata {
    std::string name;
    std::filesystem::path path;
    std::string lastUpdate;
};

class ProjectHistory {
public:
    static void AddProject(const std::string& name, const std::filesystem::path& path);
    static std::vector<ProjectMetadata> GetProjects();

private:
    static void Save(const std::vector<ProjectMetadata>& projects);
    static QString GetHistoryFilePath();
};

#endif // PROJECTHISTORY_H