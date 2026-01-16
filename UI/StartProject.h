#ifndef PULSEENGINE_STARTPROJECT_H
#define PULSEENGINE_STARTPROJECT_H

#include <QDialog>
#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QGroupBox>
#include <QSplitter>

class StartProject : public QDialog
{
    Q_OBJECT

public:
    explicit StartProject(QWidget *parent = nullptr);
    ~StartProject() = default;

    QString getProjectName() const;
    QString getProjectPath() const;
    QString getSelectedTemplate() const;

private:
    void setupUi();
    void setupStyles();
    void loadTemplates();
    void loadRecentProjects();

    QListWidget* m_sidebarList;
    QStackedWidget* m_pagesStack;

    // Recent Page Widgets
    QListWidget* m_recentList;
    QPushButton* m_openBtn;

    // New Project Page Widgets
    QListWidget* m_templateList;
    QLabel* m_templatePreview;
    QLabel* m_templateDesc;
    QLineEdit* m_nameEdit;
    QLineEdit* m_locationEdit;
    QPushButton* m_browseBtn;
    QPushButton* m_createBtn;

private slots:
    void onSidebarRowChanged(int row);
    void onTemplateSelected(QListWidgetItem* item);
    void onBrowseClicked();
    void onCreateClicked();
    void onOpenClicked();
};

#endif //PULSEENGINE_STARTPROJECT_H