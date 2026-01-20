#include "mainwindow.h"
#include "SFMLWidget.h"
#include "../EngineCore/components.h"

#include <QApplication>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QMenuBar>
#include <QDebug>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>

PulseEngineMainWindow::PulseEngineMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Pulse Engine");
    setGeometry(100, 100, 1600, 900);

    setup_dark_theme();
    create_menu_bar();

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

        m_sfmlWidget->GetScene().CreateEntity("Cube Object");
        RefreshHierarchy();
    }
}

PulseEngineMainWindow::~PulseEngineMainWindow()
{
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

    QPushButton* addEntityBtn = new QPushButton("Create Entity");
    connect(addEntityBtn, &QPushButton::clicked, this, &PulseEngineMainWindow::on_add_entity);

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

    inspector_layout->addRow("Name", m_tagEdit);
    inspector_layout->addRow("Pos X", m_posX);
    inspector_layout->addRow("Pos Y", m_posY);
    inspector_layout->addRow("Scale X", m_scaleX);
    inspector_layout->addRow("Scale Y", m_scaleY);
    inspector_layout->addRow("Rotation", m_rotation);

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
    QLabel *explorer_label = new QLabel("Explorer / Assets");
    explorer_label->setAlignment(Qt::AlignCenter);
    explorer_layout->addWidget(explorer_label);
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

void PulseEngineMainWindow::on_add_entity()
{
    if (!m_sfmlWidget) return;
    m_sfmlWidget->GetScene().CreateEntity("New Entity");
    RefreshHierarchy();
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
        m_posX->blockSignals(true); m_posX->setValue(0); m_posX->blockSignals(false);
        m_posY->blockSignals(true); m_posY->setValue(0); m_posY->blockSignals(false);
        m_scaleX->blockSignals(true); m_scaleX->setValue(1); m_scaleX->blockSignals(false);
        m_scaleY->blockSignals(true); m_scaleY->setValue(1); m_scaleY->blockSignals(false);
        m_rotation->blockSignals(true); m_rotation->setValue(0); m_rotation->blockSignals(false);
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
}

void PulseEngineMainWindow::on_inspector_change()
{
    if (m_selectedEntity == entt::null || !m_sfmlWidget) return;

    auto& registry = m_sfmlWidget->GetScene().GetRegistry();

    if (registry.all_of<TagComponent>(m_selectedEntity)) {
        std::string newTag = m_tagEdit->text().toStdString();
        if (registry.get<TagComponent>(m_selectedEntity).Tag != newTag) {
            registry.get<TagComponent>(m_selectedEntity).Tag = newTag;

            auto items = m_hierarchyTree->selectedItems();
            if(!items.isEmpty()) items[0]->setText(0, m_tagEdit->text());
        }
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

void PulseEngineMainWindow::on_save()
{
    if (!m_sfmlWidget) return;
    QString fileName = QFileDialog::getSaveFileName(this, "Save Scene", "", "JSON Files (*.json)");
    if (!fileName.isEmpty()) {
        m_sfmlWidget->GetScene().SaveScene(fileName.toStdString());
        qDebug() << "Scene saved to" << fileName;
    }
}

void PulseEngineMainWindow::on_open_scene()
{
    if (!m_sfmlWidget) return;
    QString fileName = QFileDialog::getOpenFileName(this, "Open Scene", "", "JSON Files (*.json)");
    if (!fileName.isEmpty()) {
        m_sfmlWidget->GetScene().LoadScene(fileName.toStdString());
        RefreshHierarchy();
        qDebug() << "Scene loaded from" << fileName;
    }
}

// ... (остальной код UI и setup_dark_theme без изменений, слоты-заглушки) ...

void PulseEngineMainWindow::setup_dark_theme()
{
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
    )");
}

QWidget* PulseEngineMainWindow::create_logo_widget() {
    QWidget *logo_widget = new QWidget();
    QHBoxLayout *logo_layout = new QHBoxLayout(logo_widget);
    logo_layout->setContentsMargins(10, 5, 10, 5);
    logo_layout->setSpacing(10);

    QLabel *logo_label = new QLabel();
    QPixmap logoPixmap("UI/resources/PulseEngineLogo.png");

    if (logoPixmap.isNull()) {
        logo_label->setText("Logo");
    } else {
        logo_label->setPixmap(logoPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

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
    file_menu->addAction("New Scene", this, &PulseEngineMainWindow::on_new_scene);
    file_menu->addSeparator();
    file_menu->addAction("Open Project", this, &PulseEngineMainWindow::on_open_project);
    file_menu->addAction("Open Scene", this, &PulseEngineMainWindow::on_open_scene);
    file_menu->addSeparator();
    file_menu->addAction("Save", this, &PulseEngineMainWindow::on_save, QKeySequence("Ctrl+S"));
    file_menu->addAction("Save As...", this, &PulseEngineMainWindow::on_save_as, QKeySequence("Ctrl+Shift+S"));
    file_menu->addSeparator();
    file_menu->addAction("Export", this, &PulseEngineMainWindow::on_export);
    file_menu->addAction("Import", this, &PulseEngineMainWindow::on_import);
    file_menu->addSeparator();
    file_menu->addAction("Close Project", this, &PulseEngineMainWindow::on_close_project);
    file_menu->addAction("Exit", this, &PulseEngineMainWindow::on_exit, QKeySequence("Ctrl+Q"));

    QMenu *edit_menu = menubar->addMenu("Edit");
    edit_menu->addAction("Undo", this, &PulseEngineMainWindow::on_undo, QKeySequence("Ctrl+Z"));
    edit_menu->addAction("Redo", this, &PulseEngineMainWindow::on_redo, QKeySequence("Ctrl+Y"));
    edit_menu->addSeparator();
    edit_menu->addAction("Cut", this, &PulseEngineMainWindow::on_cut, QKeySequence("Ctrl+X"));
    edit_menu->addAction("Copy", this, &PulseEngineMainWindow::on_copy, QKeySequence("Ctrl+C"));
    edit_menu->addAction("Paste", this, &PulseEngineMainWindow::on_paste, QKeySequence("Ctrl+V"));
    edit_menu->addAction("Delete", this, &PulseEngineMainWindow::on_delete, QKeySequence("Delete"));
    edit_menu->addSeparator();
    edit_menu->addAction("Duplicate", this, &PulseEngineMainWindow::on_duplicate, QKeySequence("Ctrl+D"));
    edit_menu->addSeparator();
    edit_menu->addAction("Select All", this, &PulseEngineMainWindow::on_select_all, QKeySequence("Ctrl+A"));
    edit_menu->addAction("Invert Selection", this, &PulseEngineMainWindow::on_invert_selection, QKeySequence("Ctrl+I"));
    edit_menu->addSeparator();
    edit_menu->addAction("Preferences", this, &PulseEngineMainWindow::on_preferences, QKeySequence("Ctrl+,"));
    edit_menu->addAction("Settings", this, &PulseEngineMainWindow::on_edit_settings);

    QMenu *settings_menu = menubar->addMenu("Settings");
    settings_menu->addAction("Project Settings", this, &PulseEngineMainWindow::on_project_settings);
    settings_menu->addAction("Editor Settings", this, &PulseEngineMainWindow::on_editor_settings);
    settings_menu->addSeparator();
    settings_menu->addAction("Graphics Settings", this, &PulseEngineMainWindow::on_graphics_settings);
    settings_menu->addAction("Rendering Settings", this, &PulseEngineMainWindow::on_rendering_settings);
    settings_menu->addSeparator();
    settings_menu->addAction("Input Settings", this, &PulseEngineMainWindow::on_input_settings);
    settings_menu->addAction("Controls", this, &PulseEngineMainWindow::on_controls_settings);

    QMenu *tools_menu = menubar->addMenu("Tools");
    tools_menu->addAction("Script Editor", this, &PulseEngineMainWindow::on_script_editor);
    tools_menu->addAction("Debugger", this, &PulseEngineMainWindow::on_debugger);
    tools_menu->addSeparator();
    tools_menu->addAction("Asset Manager", this, &PulseEngineMainWindow::on_asset_manager);
    tools_menu->addSeparator();
    tools_menu->addAction("Physics Tools", this, &PulseEngineMainWindow::on_physics_tools);
    tools_menu->addAction("AI Tools", this, &PulseEngineMainWindow::on_ai_tools);
    tools_menu->addAction("Pathfinding Tools", this, &PulseEngineMainWindow::on_pathfinding_tools);
    tools_menu->addSeparator();
    tools_menu->addAction("Console", this, &PulseEngineMainWindow::on_console);
    tools_menu->addAction("Log Viewer", this, &PulseEngineMainWindow::on_log_viewer);

    QMenu *view_menu = menubar->addMenu("View");
    QMenu *panels_menu = view_menu->addMenu("Toggle Panels");

    toggle_hierarchy_action = panels_menu->addAction("Hierarchy", this, &PulseEngineMainWindow::on_toggle_hierarchy);
    toggle_hierarchy_action->setCheckable(true);
    toggle_hierarchy_action->setChecked(true);

    toggle_inspector_action = panels_menu->addAction("Inspector", this, &PulseEngineMainWindow::on_toggle_inspector);
    toggle_inspector_action->setCheckable(true);
    toggle_inspector_action->setChecked(true);

    toggle_assets_action = panels_menu->addAction("Assets", this, &PulseEngineMainWindow::on_toggle_assets);
    toggle_assets_action->setCheckable(true);
    toggle_assets_action->setChecked(true);

    toggle_console_action = panels_menu->addAction("Console", this, &PulseEngineMainWindow::on_toggle_console);
    toggle_console_action->setCheckable(true);
    toggle_console_action->setChecked(true);

    toggle_explorer_action = panels_menu->addAction("Explorer", this, &PulseEngineMainWindow::on_toggle_explorer);
    toggle_explorer_action->setCheckable(true);
    toggle_explorer_action->setChecked(true);

    view_menu->addSeparator();
    fullscreen_action = view_menu->addAction("Fullscreen", this, &PulseEngineMainWindow::on_toggle_fullscreen, QKeySequence("F11"));
    fullscreen_action->setCheckable(true);

    QMenu *layouts_menu = view_menu->addMenu("Layouts");
    layouts_menu->addAction("Default Layout", this, &PulseEngineMainWindow::on_default_layout);
    layouts_menu->addAction("Coding Layout", this, &PulseEngineMainWindow::on_coding_layout);
    layouts_menu->addAction("Art Layout", this, &PulseEngineMainWindow::on_art_layout);
    layouts_menu->addAction("Design Layout", this, &PulseEngineMainWindow::on_design_layout);
    layouts_menu->addSeparator();
    layouts_menu->addAction("Save Current Layout", this, &PulseEngineMainWindow::on_save_layout);

    view_menu->addSeparator();
    QMenu *zoom_menu = view_menu->addMenu("Zoom");
    zoom_menu->addAction("Zoom In", this, &PulseEngineMainWindow::on_zoom_in, QKeySequence("Ctrl++"));
    zoom_menu->addAction("Zoom Out", this, &PulseEngineMainWindow::on_zoom_out, QKeySequence("Ctrl+-"));
    zoom_menu->addAction("Reset Zoom", this, &PulseEngineMainWindow::on_zoom_reset, QKeySequence("Ctrl+0"));
    view_menu->addAction("Frame Selected", this, &PulseEngineMainWindow::on_frame_selected, QKeySequence("F"));

    view_menu->addSeparator();
    show_grid_action = view_menu->addAction("Show Grid", this, &PulseEngineMainWindow::on_toggle_grid);
    show_grid_action->setCheckable(true);
    show_grid_action->setChecked(true);

    show_gizmos_action = view_menu->addAction("Show Gizmos", this, &PulseEngineMainWindow::on_toggle_gizmos);
    show_gizmos_action->setCheckable(true);
    show_gizmos_action->setChecked(true);

    view_menu->addSeparator();
    QMenu *camera_menu = view_menu->addMenu("Camera");
    perspective_camera_action = camera_menu->addAction("Perspective", this, &PulseEngineMainWindow::on_perspective_camera);
    perspective_camera_action->setCheckable(true);
    perspective_camera_action->setChecked(true);

    orthographic_camera_action = camera_menu->addAction("Orthographic", this, &PulseEngineMainWindow::on_orthographic_camera);
    orthographic_camera_action->setCheckable(true);

    camera_menu->addSeparator();
    camera_menu->addAction("Top View", this, &PulseEngineMainWindow::on_top_view, QKeySequence("Numpad 7"));
    camera_menu->addAction("Front View", this, &PulseEngineMainWindow::on_front_view, QKeySequence("Numpad 1"));
    camera_menu->addAction("Side View", this, &PulseEngineMainWindow::on_side_view, QKeySequence("Numpad 3"));
}

void PulseEngineMainWindow::on_new_project() {} void PulseEngineMainWindow::on_new_scene() {}
void PulseEngineMainWindow::on_open_project() {}
void PulseEngineMainWindow::on_save_as() {}
void PulseEngineMainWindow::on_export() {} void PulseEngineMainWindow::on_import() {}
void PulseEngineMainWindow::on_close_project() {}
void PulseEngineMainWindow::on_exit() { close(); }
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
void PulseEngineMainWindow::on_log_viewer() {}
void PulseEngineMainWindow::on_toggle_hierarchy() {} void PulseEngineMainWindow::on_toggle_inspector() {}
void PulseEngineMainWindow::on_toggle_assets() {} void PulseEngineMainWindow::on_toggle_console() {}
void PulseEngineMainWindow::on_toggle_explorer() {} void PulseEngineMainWindow::on_toggle_fullscreen() {}
void PulseEngineMainWindow::on_default_layout() {} void PulseEngineMainWindow::on_coding_layout() {}
void PulseEngineMainWindow::on_art_layout() {} void PulseEngineMainWindow::on_design_layout() {}
void PulseEngineMainWindow::on_save_layout() {} void PulseEngineMainWindow::on_zoom_in() {}
void PulseEngineMainWindow::on_zoom_out() {} void PulseEngineMainWindow::on_zoom_reset() {}
void PulseEngineMainWindow::on_frame_selected() {} void PulseEngineMainWindow::on_toggle_grid() {}
void PulseEngineMainWindow::on_toggle_gizmos() {} void PulseEngineMainWindow::on_perspective_camera() {}
void PulseEngineMainWindow::on_orthographic_camera() {} void PulseEngineMainWindow::on_top_view() {}
void PulseEngineMainWindow::on_front_view() {} void PulseEngineMainWindow::on_side_view() {}
void PulseEngineMainWindow::on_toolbar_edit() {} void PulseEngineMainWindow::on_toolbar_search() {}
void PulseEngineMainWindow::on_toolbar_play() {} void PulseEngineMainWindow::on_toolbar_settings() {}
void PulseEngineMainWindow::on_toolbar_tools() {} void PulseEngineMainWindow::on_scene_edit() {}
void PulseEngineMainWindow::on_scene_zoom() {} void PulseEngineMainWindow::on_scene_filter() {}
void PulseEngineMainWindow::on_scene_settings() {} void PulseEngineMainWindow::on_scene_transform() {}
void PulseEngineMainWindow::on_scene_view() {} void PulseEngineMainWindow::on_2d_view() {}
void PulseEngineMainWindow::on_view_options() {} void PulseEngineMainWindow::on_move_tool() {}
void PulseEngineMainWindow::on_rotate_tool() {} void PulseEngineMainWindow::on_scale_tool() {}
void PulseEngineMainWindow::on_grid_tool() {} void PulseEngineMainWindow::on_snap_tool() {}
void PulseEngineMainWindow::on_camera_tool() {} void PulseEngineMainWindow::on_render_tool() {}
void PulseEngineMainWindow::on_view_tool() {} void PulseEngineMainWindow::on_explorer_copy() {}
void PulseEngineMainWindow::on_explorer_open() {} void PulseEngineMainWindow::on_explorer_back() {}
void PulseEngineMainWindow::on_explorer_refresh() {} void PulseEngineMainWindow::on_explorer_filter() {}
void PulseEngineMainWindow::on_explorer_clear() {} void PulseEngineMainWindow::on_explorer_preview() {}
void PulseEngineMainWindow::on_explorer_delete() {} void PulseEngineMainWindow::on_explorer_show() {}
void PulseEngineMainWindow::on_explorer_menu() {} void PulseEngineMainWindow::on_explorer_dropdown() {}
void PulseEngineMainWindow::on_explorer_audio() {} void PulseEngineMainWindow::on_explorer_display() {}