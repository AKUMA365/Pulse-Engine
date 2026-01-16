#include "StartProject.h"
#include <QHeaderView>
#include <QMessageBox>

#include "ProjectHistory.h"

StartProject::StartProject(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Pulse Engine - Project Browser");
    resize(1100, 700);
    
    setupUi();
    setupStyles();
    loadTemplates();
    loadRecentProjects();
}

void StartProject::setupUi()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Sidebar
    QWidget* sidebarContainer = new QWidget(this);
    sidebarContainer->setFixedWidth(250);
    sidebarContainer->setObjectName("Sidebar");
    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebarContainer);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    sidebarLayout->setSpacing(0);

    // Logo Area
    QLabel* logoLabel = new QLabel("PULSE ENGINE", this);
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setFixedHeight(80);
    logoLabel->setObjectName("LogoLabel");
    sidebarLayout->addWidget(logoLabel);

    m_sidebarList = new QListWidget(this);
    m_sidebarList->addItem("Recent Projects");
    m_sidebarList->addItem("New Project");
    m_sidebarList->setCurrentRow(1); 
    sidebarLayout->addWidget(m_sidebarList);

    // Sidebar Footer
    QLabel* versionLabel = new QLabel("v1.0.0 Alpha", this);
    versionLabel->setAlignment(Qt::AlignCenter);
    versionLabel->setObjectName("VersionLabel");
    sidebarLayout->addWidget(versionLabel);
    
    mainLayout->addWidget(sidebarContainer);

    // Content Area
    m_pagesStack = new QStackedWidget(this);
    
    // PAGE 1: Recent Projects
    QWidget* recentPage = new QWidget();
    QVBoxLayout* recentLayout = new QVBoxLayout(recentPage);
    recentLayout->setContentsMargins(30, 30, 30, 30);
    
    QLabel* recentTitle = new QLabel("Recent Projects", recentPage);
    recentTitle->setObjectName("PageTitle");
    recentLayout->addWidget(recentTitle);

    m_recentList = new QListWidget(recentPage);
    recentLayout->addWidget(m_recentList);

    QHBoxLayout* recentBtnLayout = new QHBoxLayout();
    recentBtnLayout->addStretch();
    m_openBtn = new QPushButton("Open Selected", recentPage);
    m_openBtn->setFixedSize(150, 40);
    m_openBtn->setObjectName("ActionBtn");
    recentBtnLayout->addWidget(m_openBtn);
    recentLayout->addLayout(recentBtnLayout);

    m_pagesStack->addWidget(recentPage);

    // PAGE 2: New Project
    QWidget* newPage = new QWidget();
    QVBoxLayout* newLayout = new QVBoxLayout(newPage);
    newLayout->setContentsMargins(30, 30, 30, 30);

    QLabel* newTitle = new QLabel("Select Template", newPage);
    newTitle->setObjectName("PageTitle");
    newLayout->addWidget(newTitle);

    QSplitter* templateSplitter = new QSplitter(Qt::Horizontal, newPage);
    
    // Template List (Grid)
    m_templateList = new QListWidget(templateSplitter);
    m_templateList->setViewMode(QListWidget::IconMode);
    m_templateList->setIconSize(QSize(128, 128));
    m_templateList->setSpacing(15);
    m_templateList->setResizeMode(QListWidget::Adjust);
    
    // Template Details
    QWidget* detailsWidget = new QWidget(templateSplitter);
    QVBoxLayout* detailsLayout = new QVBoxLayout(detailsWidget);
    
    m_templatePreview = new QLabel("Preview", detailsWidget);
    m_templatePreview->setFixedSize(250, 150);
    m_templatePreview->setAlignment(Qt::AlignCenter);
    m_templatePreview->setStyleSheet("background-color: #151515; border: 1px solid #333; color: #555;");
    
    m_templateDesc = new QLabel("Description goes here...", detailsWidget);
    m_templateDesc->setWordWrap(true);
    m_templateDesc->setAlignment(Qt::AlignTop);
    
    detailsLayout->addWidget(m_templatePreview);
    detailsLayout->addWidget(m_templateDesc);
    detailsLayout->addStretch();
    
    templateSplitter->addWidget(m_templateList);
    templateSplitter->addWidget(detailsWidget);
    templateSplitter->setStretchFactor(0, 2);
    templateSplitter->setStretchFactor(1, 1);
    
    newLayout->addWidget(templateSplitter);

    // Project Settings Form
    QGroupBox* settingsGroup = new QGroupBox("Project Settings", newPage);
    QGridLayout* settingsGrid = new QGridLayout(settingsGroup);
    
    settingsGrid->addWidget(new QLabel("Name:"), 0, 0);
    m_nameEdit = new QLineEdit("MyNewProject", settingsGroup);
    settingsGrid->addWidget(m_nameEdit, 0, 1);

    settingsGrid->addWidget(new QLabel("Location:"), 1, 0);
    QHBoxLayout* pathLayout = new QHBoxLayout();
    m_locationEdit = new QLineEdit(settingsGroup);
    m_browseBtn = new QPushButton("...", settingsGroup);
    m_browseBtn->setFixedWidth(40);
    pathLayout->addWidget(m_locationEdit);
    pathLayout->addWidget(m_browseBtn);
    settingsGrid->addLayout(pathLayout, 1, 1);

    newLayout->addWidget(settingsGroup);

    // Footer Buttons
    QHBoxLayout* footerLayout = new QHBoxLayout();
    QPushButton* cancelBtn = new QPushButton("Cancel", newPage);
    cancelBtn->setFixedSize(120, 40);
    
    m_createBtn = new QPushButton("Create Project", newPage);
    m_createBtn->setFixedSize(160, 40);
    m_createBtn->setObjectName("PrimaryBtn");

    footerLayout->addStretch();
    footerLayout->addWidget(cancelBtn);
    footerLayout->addWidget(m_createBtn);
    
    newLayout->addLayout(footerLayout);
    m_pagesStack->addWidget(newPage);

    mainLayout->addWidget(m_pagesStack);

    // Connections
    connect(m_sidebarList, &QListWidget::currentRowChanged, this, &StartProject::onSidebarRowChanged);
    connect(m_templateList, &QListWidget::itemClicked, this, &StartProject::onTemplateSelected);
    connect(m_browseBtn, &QPushButton::clicked, this, &StartProject::onBrowseClicked);
    connect(m_createBtn, &QPushButton::clicked, this, &StartProject::onCreateClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_openBtn, &QPushButton::clicked, this, &StartProject::onOpenClicked);
}

void StartProject::setupStyles()
{
    setStyleSheet(R"(
        QDialog { background-color: #1e2126; color: #e0e0e0; font-family: 'Segoe UI', sans-serif; }
        
        /* Sidebar */
        #Sidebar { background-color: #15171b; }
        #LogoLabel { color: #42a5f5; font-size: 18px; font-weight: bold; border-bottom: 1px solid #2a2d32; }
        #VersionLabel { color: #666; padding: 10px; font-size: 11px; }
        
        QListWidget { background-color: transparent; border: none; outline: none; }
        QListWidget::item { padding: 12px 20px; color: #b0b0b0; border-left: 3px solid transparent; }
        QListWidget::item:selected { background-color: #25282d; color: #fff; border-left: 3px solid #42a5f5; }
        QListWidget::item:hover { background-color: #1e2126; }
        
        /* Content */
        #PageTitle { font-size: 24px; color: #fff; margin-bottom: 15px; }
        
        QLineEdit { background-color: #111; border: 1px solid #333; border-radius: 4px; padding: 8px; color: #fff; }
        QLineEdit:focus { border: 1px solid #42a5f5; }
        
        QGroupBox { border: 1px solid #333; border-radius: 6px; margin-top: 20px; padding-top: 15px; font-weight: bold; }
        QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; color: #b0b0b0; }
        
        /* Buttons */
        QPushButton { background-color: #333; border: none; border-radius: 4px; padding: 8px; color: #fff; }
        QPushButton:hover { background-color: #444; }
        QPushButton:pressed { background-color: #222; }
        
        #PrimaryBtn { background-color: #42a5f5; color: #fff; font-weight: bold; }
        #PrimaryBtn:hover { background-color: #64b5f6; }
        #ActionBtn { background-color: #42a5f5; font-weight: bold; }
    )");
    
    // Sidebar specific style for larger text
    m_sidebarList->setStyleSheet("font-size: 14px;");
}

void StartProject::loadTemplates()
{
    // Clear list
    m_templateList->clear();

    // Blank Project
    QListWidgetItem* blankItem = new QListWidgetItem("Blank Project");
    // TODO: Set Icon: blankItem->setIcon(QIcon(":/icons/blank.png"));
    blankItem->setData(Qt::UserRole, "A clean slate for your project.");
    blankItem->setData(Qt::UserRole + 1, "blank");
    m_templateList->addItem(blankItem);

    // 2D Project
    QListWidgetItem* twoDItem = new QListWidgetItem("2D Platformer");
    // TODO: Set Icon: twoDItem->setIcon(QIcon(":/icons/2d.png"));
    twoDItem->setData(Qt::UserRole, "A basic 2D game setup with physics.");
    twoDItem->setData(Qt::UserRole + 1, "2d");
    m_templateList->addItem(twoDItem);

    // Select first by default
    m_templateList->setCurrentRow(0);
    onTemplateSelected(m_templateList->item(0));
}

void StartProject::loadRecentProjects()
{
    m_recentList->clear();
    auto projects = ProjectHistory::GetProjects();

    for (const auto& meta : projects) {
        QString text = QString::fromStdString(meta.name);
        QString date = QString::fromStdString(meta.lastUpdate);

        QListWidgetItem* item = new QListWidgetItem();
        item->setText(text + " (" + date + ")");
        item->setToolTip(QString::fromStdString(meta.path.string()));

        item->setData(Qt::UserRole, QString::fromStdString(meta.path.string()));

        m_recentList->addItem(item);
    }
}

void StartProject::onSidebarRowChanged(int row)
{
    m_pagesStack->setCurrentIndex(row);
}

void StartProject::onTemplateSelected(QListWidgetItem* item)
{
    if(!item) return;
    QString desc = item->data(Qt::UserRole).toString();
    m_templateDesc->setText(desc);
    
    // TODO: Update preview image based on template type
}

void StartProject::onBrowseClicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Project Location", 
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        m_locationEdit->setText(dir);
    }
}

void StartProject::onCreateClicked()
{
    QString name = m_nameEdit->text();
    QString path = m_locationEdit->text();
    
    if (name.isEmpty() || path.isEmpty()) {
        QMessageBox::warning(this, "Error", "Project name and location cannot be empty.");
        return;
    }

    // TODO: Check if directory is valid and writable
    // TODO: Check if project already exists at path
    // TODO: Copy template files to new location
    // TODO: Generate project configuration file (.pulse)
    
    accept(); // Close dialog with success result
}

void StartProject::onOpenClicked()
{
    QListWidgetItem* current = m_recentList->currentItem();
    if (current) {
        // TODO: Validate selected project path
        // TODO: Load project context
        accept();
    } else {
        // Fallback to file dialog
        QString fileName = QFileDialog::getOpenFileName(this, "Open Pulse Project",
                                                        QDir::homePath(),
                                                        "Pulse Projects (*.pulse *.pproject)");
        if (!fileName.isEmpty()) {
            // TODO: Load project from file
            accept();
        }
    }
}

QString StartProject::getProjectName() const { return m_nameEdit->text(); }
QString StartProject::getProjectPath() const { return m_locationEdit->text(); }
QString StartProject::getSelectedTemplate() const 
{
    if (m_templateList->currentItem())
        return m_templateList->currentItem()->data(Qt::UserRole + 1).toString();
    return "";
}