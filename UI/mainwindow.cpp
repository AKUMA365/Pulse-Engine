#include "mainwindow.h"
#include "SFMLWidget.h"
#include "../EngineCore/components.h"
#include "StartProject.h"
#include "ScriptEditor.h"

#include <QApplication>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QMenuBar>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QToolBar>
#include <QComboBox>
#include <QMenu>
#include <QInputDialog>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

PulseEngineMainWindow::PulseEngineMainWindow(const std::string& projectPath, QWidget *parent)
    : QMainWindow(parent), m_projectPath(projectPath)
{
    setWindowTitle("Pulse Engine");
    setGeometry(100, 100, 1600, 900);

    setup_dark_theme();
    create_menu_bar();
    create_toolbar();

    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    QHBoxLayout *main_layout = new QHBoxLayout(central_widget);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);

    QSplitter *main_splitter = new QSplitter(Qt::Horizontal);

    QWidget *left_panel = create_left_panel();
    QSplitter *center_panel = create_center_panel();

    main_splitter->addWidget(left_panel);
    main_splitter->addWidget(center_panel);
    main_splitter->setSizes({280, 1320});

    main_layout->addWidget(main_splitter);

    if (m_sfmlWidget) {
        connect(m_sfmlWidget, &SFMLWidget::entitySelected, this, &PulseEngineMainWindow::on_scene_entity_selected);

        if (!m_projectPath.empty()) {
            std::string scenePath = m_projectPath + "/scene.json";
            if (fs::exists(scenePath)) {
                m_sfmlWidget->GetScene().LoadScene(scenePath);
            } else {
                m_sfmlWidget->GetScene().CreateEntity("Cube Object");
            }
            setWindowTitle("Pulse Engine - " + QString::fromStdString(fs::path(m_projectPath).filename().string()));
        } else {
            m_sfmlWidget->GetScene().CreateEntity("Cube Object");
        }

        RefreshHierarchy();
    }
}

PulseEngineMainWindow::~PulseEngineMainWindow()
{
}

void PulseEngineMainWindow::create_toolbar()
{
    QToolBar* toolbar = addToolBar("Main Toolbar");
    toolbar->setMovable(false);

    QAction* playAction = toolbar->addAction("Play");
    playAction->setIcon(QIcon::fromTheme("media-playback-start"));
    connect(playAction, &QAction::triggered, this, &PulseEngineMainWindow::on_play_scene);

    QAction* stopAction = toolbar->addAction("Stop");
    stopAction->setIcon(QIcon::fromTheme("media-playback-stop"));
    connect(stopAction, &QAction::triggered, this, &PulseEngineMainWindow::on_stop_scene);
}

QWidget* PulseEngineMainWindow::create_left_panel()
{
    QWidget *left_widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(left_widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);

    QGroupBox *hierarchy_group = new QGroupBox("Hierarchy");
    QVBoxLayout *hierarchy_layout = new QVBoxLayout(hierarchy_group);

    m_hierarchyTree = new QTreeWidget();
    m_hierarchyTree->setHeaderHidden(true);
    m_hierarchyTree->setIndentation(10);
    connect(m_hierarchyTree, &QTreeWidget::itemSelectionChanged, this, &PulseEngineMainWindow::on_hierarchy_select);

    QPushButton* addEntityBtn = new QPushButton("Add Entity");
    connect(addEntityBtn, &QPushButton::clicked, this, &PulseEngineMainWindow::on_add_entity_click);

    hierarchy_layout->addWidget(addEntityBtn);
    hierarchy_layout->addWidget(m_hierarchyTree);

    QGroupBox *inspector_group = new QGroupBox("Inspector");
    QFormLayout *inspector_layout = new QFormLayout(inspector_group);
    inspector_layout->setLabelAlignment(Qt::AlignLeft);

    m_tagEdit = new QLineEdit();
    connect(m_tagEdit, &QLineEdit::editingFinished, this, &PulseEngineMainWindow::on_inspector_change);

    m_posX = new QDoubleSpinBox(); m_posX->setRange(-99999, 99999);
    m_posY = new QDoubleSpinBox(); m_posY->setRange(-99999, 99999);
    m_scaleX = new QDoubleSpinBox(); m_scaleX->setRange(-99999, 99999); m_scaleX->setSingleStep(0.1);
    m_scaleY = new QDoubleSpinBox(); m_scaleY->setRange(-99999, 99999); m_scaleY->setSingleStep(0.1);
    m_rotation = new QDoubleSpinBox(); m_rotation->setRange(0, 360);

    connect(m_posX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PulseEngineMainWindow::on_inspector_change);
    connect(m_posY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PulseEngineMainWindow::on_inspector_change);
    connect(m_scaleX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PulseEngineMainWindow::on_inspector_change);
    connect(m_scaleY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PulseEngineMainWindow::on_inspector_change);
    connect(m_rotation, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PulseEngineMainWindow::on_inspector_change);

    m_scriptPathEdit = new QLineEdit();
    m_scriptPathEdit->setReadOnly(true);

    m_scriptBrowseBtn = new QPushButton("...");
    m_scriptBrowseBtn->setFixedWidth(30);
    connect(m_scriptBrowseBtn, &QPushButton::clicked, this, &PulseEngineMainWindow::on_add_script_clicked);

    m_scriptEditBtn = new QPushButton("Edit");
    m_scriptEditBtn->setFixedWidth(40);
    connect(m_scriptEditBtn, &QPushButton::clicked, this, &PulseEngineMainWindow::on_edit_script_clicked);

    QHBoxLayout* scriptLayout = new QHBoxLayout();
    scriptLayout->addWidget(m_scriptPathEdit);
    scriptLayout->addWidget(m_scriptBrowseBtn);
    scriptLayout->addWidget(m_scriptEditBtn);

    inspector_layout->addRow("Name", m_tagEdit);
    inspector_layout->addRow("Pos X", m_posX);
    inspector_layout->addRow("Pos Y", m_posY);
    inspector_layout->addRow("Scale X", m_scaleX);
    inspector_layout->addRow("Scale Y", m_scaleY);
    inspector_layout->addRow("Rotation", m_rotation);
    inspector_layout->addRow("Script", scriptLayout);

    layout->addWidget(hierarchy_group);
    layout->addWidget(inspector_group);

    return left_widget;
}

QSplitter* PulseEngineMainWindow::create_center_panel()
{
    QSplitter *center_splitter = new QSplitter(Qt::Vertical);

    QWidget *scene_container = new QWidget();
    QVBoxLayout *scene_layout = new QVBoxLayout(scene_container);
    scene_layout->setContentsMargins(0,0,0,0);

    m_sfmlWidget = new SFMLWidget();
    scene_layout->addWidget(m_sfmlWidget);

    center_splitter->addWidget(scene_container);

    QWidget *explorer_widget = new QWidget();
    QVBoxLayout *explorer_layout = new QVBoxLayout(explorer_widget);
    explorer_layout->setContentsMargins(0, 0, 0, 0);

    QLabel *explorer_label = new QLabel("Project Files");
    explorer_label->setStyleSheet("padding: 5px; background: #111; color: #888; font-weight: bold;");
    explorer_layout->addWidget(explorer_label);

    m_fileModel = new QFileSystemModel(this);
    if (!m_projectPath.empty()) {
        m_fileModel->setRootPath(QString::fromStdString(m_projectPath));
    } else {
        m_fileModel->setRootPath(QDir::homePath());
    }

    m_fileView = new QListView();
    m_fileView->setModel(m_fileModel);

    if (!m_projectPath.empty()) {
        m_fileView->setRootIndex(m_fileModel->index(QString::fromStdString(m_projectPath)));
    } else {
        m_fileView->setRootIndex(m_fileModel->index(QDir::homePath()));
    }

    m_fileView->setViewMode(QListView::IconMode);
    m_fileView->setIconSize(QSize(64, 64));
    m_fileView->setGridSize(QSize(80, 80));
    m_fileView->setResizeMode(QListView::Adjust);
    m_fileView->setUniformItemSizes(true);
    m_fileView->setMovement(QListView::Static);

    explorer_layout->addWidget(m_fileView);
    center_splitter->addWidget(explorer_widget);

    center_splitter->setSizes({600, 300});

    return center_splitter;
}

void PulseEngineMainWindow::RefreshHierarchy()
{
    if (!m_hierarchyTree || !m_sfmlWidget) return;

    m_hierarchyTree->clear();
    auto& registry = m_sfmlWidget->GetScene().GetRegistry();

    auto view = registry.view<TagComponent>();
    for(auto entity : view) {
        auto& tag = view.get<TagComponent>(entity);
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, QString::fromStdString(tag.Tag));
        item->setData(0, Qt::UserRole, (qulonglong)entity);
        m_hierarchyTree->addTopLevelItem(item);
    }
}

void PulseEngineMainWindow::on_add_entity_click()
{
    QMenu menu(this);
    menu.addAction("Cube (Preset)", this, &PulseEngineMainWindow::on_add_primitive_cube);
    menu.addAction("Custom Object...", this, &PulseEngineMainWindow::on_add_custom_object);
    menu.exec(QCursor::pos());
}

void PulseEngineMainWindow::on_add_primitive_cube()
{
    if (!m_sfmlWidget) return;
    m_sfmlWidget->GetScene().CreateEntity("Cube");
    RefreshHierarchy();
}

void PulseEngineMainWindow::on_add_custom_object()
{
    if (m_projectPath.empty()) {
        QMessageBox::warning(this, "Error", "Save or create a project first!");
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this, "Select Image", QDir::homePath(), "Images (*.png *.jpg *.jpeg)");
    if (fileName.isEmpty()) return;

    fs::path sourcePath(fileName.toStdString());
    fs::path destPath = fs::path(m_projectPath) / sourcePath.filename();

    try {
        if (!fs::exists(destPath)) {
            fs::copy(sourcePath, destPath);
        }
    } catch (fs::filesystem_error& e) {
        QMessageBox::critical(this, "Error", QString::fromStdString(e.what()));
        return;
    }

    m_sfmlWidget->GetScene().CreateEntityWithTexture(sourcePath.stem().string(), destPath.string());
    RefreshHierarchy();
}

void PulseEngineMainWindow::on_add_script_clicked()
{
    if (m_selectedEntity == entt::null || !m_sfmlWidget) return;

    QString fileName = QFileDialog::getOpenFileName(this, "Select Script", QString::fromStdString(m_projectPath), "Pulse Scripts (*.pulse *.txt)");
    if (!fileName.isEmpty()) {
        auto& registry = m_sfmlWidget->GetScene().GetRegistry();
        auto& script = registry.get_or_emplace<NativeScriptComponent>(m_selectedEntity);
        script.FilePath = fileName.toStdString();
        UpdateInspector();
    }
}

void PulseEngineMainWindow::on_create_new_script()
{
    if (m_projectPath.empty()) {
        QMessageBox::warning(this, "Error", "Project not saved!");
        return;
    }

    bool ok;
    QString text = QInputDialog::getText(this, "New Script", "Script Name:", QLineEdit::Normal, "NewScript", &ok);
    if (ok && !text.isEmpty()) {
        std::string filename = text.toStdString();
        if (filename.find(".pulse") == std::string::npos) filename += ".pulse";

        fs::path fullPath = fs::path(m_projectPath) / filename;
        std::ofstream outfile(fullPath);
        outfile.close();

        ScriptEditor* editor = new ScriptEditor(fullPath.string(), this);
        editor->show();
    }
}

void PulseEngineMainWindow::on_edit_script_clicked()
{
    std::string path = m_scriptPathEdit->text().toStdString();
    if (!path.empty() && fs::exists(path)) {
        ScriptEditor* editor = new ScriptEditor(path, this);
        editor->show();
    }
}

void PulseEngineMainWindow::on_hierarchy_select()
{
    auto items = m_hierarchyTree->selectedItems();
    if (items.isEmpty()) {
        m_selectedEntity = entt::null;
        UpdateInspector();
        return;
    }
    m_selectedEntity = (entt::entity)items[0]->data(0, Qt::UserRole).toULongLong();
    UpdateInspector();
}

void PulseEngineMainWindow::on_scene_entity_selected(size_t entityID)
{
    if ((entt::entity)entityID == entt::null) {
        m_hierarchyTree->clearSelection();
        return;
    }

    for(int i = 0; i < m_hierarchyTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem* item = m_hierarchyTree->topLevelItem(i);
        if (item->data(0, Qt::UserRole).toULongLong() == (qulonglong)entityID) {
            m_hierarchyTree->setCurrentItem(item);
            return;
        }
    }
}

void PulseEngineMainWindow::UpdateInspector()
{
    if (m_selectedEntity == entt::null) {
        m_tagEdit->clear();
        m_posX->setValue(0);
        m_posY->setValue(0);
        m_scaleX->setValue(1);
        m_scaleY->setValue(1);
        m_rotation->setValue(0);
        m_scriptPathEdit->clear();
        return;
    }

    if (!m_sfmlWidget) return;
    auto& registry = m_sfmlWidget->GetScene().GetRegistry();

    if (registry.all_of<TagComponent>(m_selectedEntity)) {
        m_tagEdit->setText(QString::fromStdString(registry.get<TagComponent>(m_selectedEntity).Tag));
    }

    if (registry.all_of<TransformComponent>(m_selectedEntity)) {
        auto& tc = registry.get<TransformComponent>(m_selectedEntity);
        m_posX->blockSignals(true); m_posX->setValue(tc.Position.x); m_posX->blockSignals(false);
        m_posY->blockSignals(true); m_posY->setValue(tc.Position.y); m_posY->blockSignals(false);
        m_scaleX->blockSignals(true); m_scaleX->setValue(tc.Scale.x); m_scaleX->blockSignals(false);
        m_scaleY->blockSignals(true); m_scaleY->setValue(tc.Scale.y); m_scaleY->blockSignals(false);
        m_rotation->blockSignals(true); m_rotation->setValue(tc.Rotation); m_rotation->blockSignals(false);
    }

    if (registry.all_of<NativeScriptComponent>(m_selectedEntity)) {
        m_scriptPathEdit->setText(QString::fromStdString(registry.get<NativeScriptComponent>(m_selectedEntity).FilePath));
    } else {
        m_scriptPathEdit->clear();
    }
}

void PulseEngineMainWindow::on_inspector_change()
{
    if (m_selectedEntity == entt::null || !m_sfmlWidget) return;

    auto& registry = m_sfmlWidget->GetScene().GetRegistry();

    if (registry.all_of<TagComponent>(m_selectedEntity)) {
        registry.get<TagComponent>(m_selectedEntity).Tag = m_tagEdit->text().toStdString();
        auto items = m_hierarchyTree->selectedItems();
        if(!items.isEmpty()) items[0]->setText(0, m_tagEdit->text());
    }

    if (registry.all_of<TransformComponent>(m_selectedEntity)) {
        auto& tc = registry.get<TransformComponent>(m_selectedEntity);
        tc.Position.x = m_posX->value();
        tc.Position.y = m_posY->value();
        tc.Scale.x = m_scaleX->value();
        tc.Scale.y = m_scaleY->value();
        tc.Rotation = m_rotation->value();
    }
}

void PulseEngineMainWindow::on_play_scene() {
    if(m_sfmlWidget) m_sfmlWidget->GetScene().StartRuntime();
}

void PulseEngineMainWindow::on_stop_scene() {
    if(m_sfmlWidget) {
        m_sfmlWidget->GetScene().StopRuntime();
        RefreshHierarchy();
        UpdateInspector();
    }
}

void PulseEngineMainWindow::on_save()
{
    if (!m_sfmlWidget) return;
    if (!m_projectPath.empty()) {
        std::string sceneFile = m_projectPath + "/scene.json";
        m_sfmlWidget->GetScene().SaveScene(sceneFile);
        QMessageBox::information(this, "Pulse Engine", "Project saved!");
    } else {
        on_save_as();
    }
}

void PulseEngineMainWindow::on_save_as()
{
    if (!m_sfmlWidget) return;
    QString fileName = QFileDialog::getSaveFileName(this, "Save Scene As...", "", "JSON Files (*.json)");
    if (!fileName.isEmpty()) {
        m_sfmlWidget->GetScene().SaveScene(fileName.toStdString());
    }
}

void PulseEngineMainWindow::on_new_project() {
    StartProject* start = new StartProject();
    this->close();
    start->show();
}

void PulseEngineMainWindow::on_open_project() {
    StartProject* start = new StartProject();
    this->close();
    start->show();
}

void PulseEngineMainWindow::on_new_scene() {} void PulseEngineMainWindow::on_open_scene() {}
void PulseEngineMainWindow::on_export() {} void PulseEngineMainWindow::on_import() {}
void PulseEngineMainWindow::on_close_project() {} void PulseEngineMainWindow::on_exit() { close(); }
void PulseEngineMainWindow::on_undo() {} void PulseEngineMainWindow::on_redo() {}
void PulseEngineMainWindow::on_cut() {} void PulseEngineMainWindow::on_copy() {}
void PulseEngineMainWindow::on_paste() {} void PulseEngineMainWindow::on_delete() {}
void PulseEngineMainWindow::on_duplicate() {} void PulseEngineMainWindow::on_select_all() {}
void PulseEngineMainWindow::on_invert_selection() {} void PulseEngineMainWindow::on_preferences() {}
void PulseEngineMainWindow::on_edit_settings() {} void PulseEngineMainWindow::on_project_settings() {}
void PulseEngineMainWindow::on_editor_settings() {} void PulseEngineMainWindow::on_graphics_settings() {}
void PulseEngineMainWindow::on_rendering_settings() {} void PulseEngineMainWindow::on_input_settings() {}
void PulseEngineMainWindow::on_controls_settings() {} void PulseEngineMainWindow::on_script_editor() {}
void PulseEngineMainWindow::on_debugger() {} void PulseEngineMainWindow::on_asset_manager() {}
void PulseEngineMainWindow::on_physics_tools() {} void PulseEngineMainWindow::on_ai_tools() {}
void PulseEngineMainWindow::on_pathfinding_tools() {} void PulseEngineMainWindow::on_console() {}
void PulseEngineMainWindow::on_log_viewer() {} void PulseEngineMainWindow::on_toggle_hierarchy() {}
void PulseEngineMainWindow::on_toggle_inspector() {} void PulseEngineMainWindow::on_toggle_assets() {}
void PulseEngineMainWindow::on_toggle_console() {} void PulseEngineMainWindow::on_toggle_explorer() {}
void PulseEngineMainWindow::on_toggle_fullscreen() {} void PulseEngineMainWindow::on_default_layout() {}
void PulseEngineMainWindow::on_coding_layout() {} void PulseEngineMainWindow::on_art_layout() {}
void PulseEngineMainWindow::on_design_layout() {} void PulseEngineMainWindow::on_save_layout() {}
void PulseEngineMainWindow::on_zoom_in() {} void PulseEngineMainWindow::on_zoom_out() {}
void PulseEngineMainWindow::on_zoom_reset() {} void PulseEngineMainWindow::on_frame_selected() {}
void PulseEngineMainWindow::on_toggle_grid() {} void PulseEngineMainWindow::on_toggle_gizmos() {}
void PulseEngineMainWindow::on_perspective_camera() {} void PulseEngineMainWindow::on_orthographic_camera() {}
void PulseEngineMainWindow::on_top_view() {} void PulseEngineMainWindow::on_front_view() {}
void PulseEngineMainWindow::on_side_view() {} void PulseEngineMainWindow::on_toolbar_edit() {}
void PulseEngineMainWindow::on_toolbar_search() {} void PulseEngineMainWindow::on_toolbar_play() {}
void PulseEngineMainWindow::on_toolbar_settings() {} void PulseEngineMainWindow::on_toolbar_tools() {}
void PulseEngineMainWindow::on_scene_edit() {} void PulseEngineMainWindow::on_scene_zoom() {}
void PulseEngineMainWindow::on_scene_filter() {} void PulseEngineMainWindow::on_scene_settings() {}
void PulseEngineMainWindow::on_scene_transform() {} void PulseEngineMainWindow::on_scene_view() {}
void PulseEngineMainWindow::on_2d_view() {} void PulseEngineMainWindow::on_view_options() {}
void PulseEngineMainWindow::on_move_tool() {} void PulseEngineMainWindow::on_rotate_tool() {}
void PulseEngineMainWindow::on_scale_tool() {} void PulseEngineMainWindow::on_grid_tool() {}
void PulseEngineMainWindow::on_snap_tool() {} void PulseEngineMainWindow::on_camera_tool() {}
void PulseEngineMainWindow::on_render_tool() {} void PulseEngineMainWindow::on_view_tool() {}
void PulseEngineMainWindow::on_explorer_copy() {} void PulseEngineMainWindow::on_explorer_open() {}
void PulseEngineMainWindow::on_explorer_back() {} void PulseEngineMainWindow::on_explorer_refresh() {}
void PulseEngineMainWindow::on_explorer_filter() {} void PulseEngineMainWindow::on_explorer_clear() {}
void PulseEngineMainWindow::on_explorer_preview() {} void PulseEngineMainWindow::on_explorer_delete() {}
void PulseEngineMainWindow::on_explorer_show() {} void PulseEngineMainWindow::on_explorer_menu() {}
void PulseEngineMainWindow::on_explorer_dropdown() {} void PulseEngineMainWindow::on_explorer_audio() {}
void PulseEngineMainWindow::on_explorer_display() {}

void PulseEngineMainWindow::setup_dark_theme() {
    QPalette dark_palette;
    dark_palette.setColor(QPalette::Window, QColor(5, 5, 5));
    dark_palette.setColor(QPalette::WindowText, QColor(255, 255, 255));
    dark_palette.setColor(QPalette::Base, QColor(10, 10, 10));
    dark_palette.setColor(QPalette::AlternateBase, QColor(15, 15, 15));
    dark_palette.setColor(QPalette::ToolTipBase, QColor(255, 255, 255));
    dark_palette.setColor(QPalette::ToolTipText, QColor(255, 255, 255));
    dark_palette.setColor(QPalette::Text, QColor(220, 220, 220));
    dark_palette.setColor(QPalette::Button, QColor(20, 20, 20));
    dark_palette.setColor(QPalette::ButtonText, QColor(255, 255, 255));
    dark_palette.setColor(QPalette::BrightText, Qt::red);
    dark_palette.setColor(QPalette::Link, QColor(76, 214, 192));
    dark_palette.setColor(QPalette::Highlight, QColor(76, 214, 192));
    dark_palette.setColor(QPalette::HighlightedText, Qt::black);
    setPalette(dark_palette);
    setStyleSheet(R"(
        QMainWindow { background-color: #050505; font-family: 'Segoe UI', 'Roboto', sans-serif; }
        QTreeWidget { background-color: #0a0a0a; border: 1px solid #222; padding: 5px; font-size: 13px; }
        QTreeWidget::item { padding: 4px; color: #888; }
        QTreeWidget::item:selected { background-color: #0f1212; color: #4cd6c0; border-left: 2px solid #4cd6c0; }
        QTreeWidget::item:hover { background-color: #111; color: #ccc; }
        QLabel { color: #e0e0e0; }
        QPushButton { background-color: transparent; border: 1px solid #333; border-radius: 2px; padding: 6px 12px; color: #aaa; font-weight: 600; text-transform: uppercase; font-size: 11px; letter-spacing: 0.5px; }
        QPushButton:hover { border-color: #666; color: #fff; background-color: #111; }
        QPushButton:pressed { background-color: #222; }
        QLineEdit { background-color: #080808; border: 1px solid #333; padding: 6px; border-radius: 2px; color: #fff; font-family: 'Consolas', monospace; }
        QLineEdit:focus { border: 1px solid #4cd6c0; background-color: #0c0c0c; }
        QSpinBox, QDoubleSpinBox { background-color: #080808; border: 1px solid #333; padding: 4px; border-radius: 2px; color: #fff; }
        QSpinBox:focus, QDoubleSpinBox:focus { border: 1px solid #4cd6c0; }
        QGroupBox { border: 1px solid #222; border-radius: 4px; margin-top: 15px; padding-top: 15px; color: #888; font-weight: bold; }
        QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; color: #4cd6c0; background-color: #050505; }
        QSplitter::handle { background-color: #222; width: 1px; }
        QListView { background-color: #0a0a0a; border: 1px solid #222; color: #ccc; }
        QListView::item { padding: 5px; }
        QListView::item:selected { background-color: #222; color: #4cd6c0; border: 1px solid #4cd6c0; }
    )");
}

QWidget* PulseEngineMainWindow::create_logo_widget() {
    QWidget *logo_widget = new QWidget();
    QHBoxLayout *logo_layout = new QHBoxLayout(logo_widget);
    logo_layout->setContentsMargins(10, 5, 10, 5);
    logo_layout->setSpacing(10);
    QLabel *logo_label = new QLabel();
    QPixmap logoPixmap("UI/resources/PulseEngineLogo.png");
    if (logoPixmap.isNull()) { logo_label->setText("Logo"); }
    else { logo_label->setPixmap(logoPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation)); }
    QLabel *title_label = new QLabel("Pulse Engine");
    title_label->setStyleSheet("color: #5ab9ff; font-weight: bold; font-size: 16px;");
    logo_layout->addWidget(logo_label);
    logo_layout->addWidget(title_label);
    logo_layout->addStretch();
    return logo_widget;
}

void PulseEngineMainWindow::create_menu_bar() {
    QMenuBar *menubar = menuBar();
    menubar->setNativeMenuBar(false);
    menubar->setCornerWidget(create_logo_widget(), Qt::TopLeftCorner);
    QMenu *file_menu = menubar->addMenu("File");
    file_menu->addAction("New Project", this, &PulseEngineMainWindow::on_new_project);
    file_menu->addAction("Open Project", this, &PulseEngineMainWindow::on_open_project);
    file_menu->addAction("New Script", this, &PulseEngineMainWindow::on_create_new_script);
    file_menu->addAction("Save", this, &PulseEngineMainWindow::on_save);
    file_menu->addAction("Exit", this, &PulseEngineMainWindow::on_exit);
    QMenu *tools_menu = menubar->addMenu("Tools");
    tools_menu->addAction("Play", this, &PulseEngineMainWindow::on_play_scene);
    tools_menu->addAction("Stop", this, &PulseEngineMainWindow::on_stop_scene);
}