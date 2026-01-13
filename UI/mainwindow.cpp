//
// Created by Dima Semchenko on 13.01.2026.
//

#include "mainwindow.h"

PulseEngineMainWindow::PulseEngineMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Pulse Engine");
    setGeometry(100, 100, 1600, 900);

    // Set dark theme
    setup_dark_theme();

    // Create menu bar
    create_menu_bar();

    // Create main toolbar

    // Create central widget with main layout
    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    QHBoxLayout *main_layout = new QHBoxLayout(central_widget);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);

    // Create main splitter
    QSplitter *main_splitter = new QSplitter(Qt::Horizontal);

    // Left panel (Hierarchy + Inspector)
    QWidget *left_panel = create_left_panel();

    // Center panel (Scene View + Explorer)
    QSplitter *center_panel = create_center_panel();

    main_splitter->addWidget(left_panel);
    main_splitter->addWidget(center_panel);
    main_splitter->setSizes({280, 1320});

    main_layout->addWidget(main_splitter);
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

    // Hierarchy
    QGroupBox *hierarchy_group = new QGroupBox("Hierarchy");
    QVBoxLayout *hierarchy_layout = new QVBoxLayout(hierarchy_group);
    QTreeWidget *hierarchy_tree = new QTreeWidget();
    hierarchy_tree->setHeaderHidden(true);
    hierarchy_layout->addWidget(hierarchy_tree);

    // Inspector
    QGroupBox *inspector_group = new QGroupBox("Inspector");
    QVBoxLayout *inspector_layout = new QVBoxLayout(inspector_group);
    QLabel *inspector_label = new QLabel("Select object to see properties");
    inspector_layout->addWidget(inspector_label);

    // Добавляем в общий layout
    layout->addWidget(hierarchy_group);
    layout->addWidget(inspector_group);

    return left_widget;
}

QSplitter* PulseEngineMainWindow::create_center_panel()
{
    QSplitter *center_splitter = new QSplitter(Qt::Vertical);

    // Scene View placeholder
    QWidget *scene_widget = new QWidget();
    QVBoxLayout *scene_layout = new QVBoxLayout(scene_widget);
    QLabel *scene_label = new QLabel("Scene View Placeholder");
    scene_label->setAlignment(Qt::AlignCenter);
    scene_layout->addWidget(scene_label);
    center_splitter->addWidget(scene_widget);

    // Explorer / Assets placeholder
    QWidget *explorer_widget = new QWidget();
    QVBoxLayout *explorer_layout = new QVBoxLayout(explorer_widget);
    QLabel *explorer_label = new QLabel("Explorer / Assets");
    explorer_label->setAlignment(Qt::AlignCenter);
    explorer_layout->addWidget(explorer_label);
    center_splitter->addWidget(explorer_widget);

    center_splitter->setSizes({600, 300});

    return center_splitter;
}

void PulseEngineMainWindow::setup_dark_theme()
{
    /* Apply dark theme to the application */
    QPalette dark_palette;
    dark_palette.setColor(QPalette::Window, QColor(30, 33, 38));
    dark_palette.setColor(QPalette::WindowText, QColor(200, 200, 200));
    dark_palette.setColor(QPalette::Base, QColor(25, 28, 32));
    dark_palette.setColor(QPalette::AlternateBase, QColor(35, 38, 42));
    dark_palette.setColor(QPalette::ToolTipBase, QColor(200, 200, 200));
    dark_palette.setColor(QPalette::ToolTipText, QColor(200, 200, 200));
    dark_palette.setColor(QPalette::Text, QColor(200, 200, 200));
    dark_palette.setColor(QPalette::Button, QColor(40, 43, 48));
    dark_palette.setColor(QPalette::ButtonText, QColor(200, 200, 200));
    dark_palette.setColor(QPalette::BrightText, Qt::red);
    dark_palette.setColor(QPalette::Link, QColor(42, 130, 218));
    dark_palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    dark_palette.setColor(QPalette::HighlightedText, Qt::black);

    setPalette(dark_palette);

    // Apply stylesheet for additional styling
    setStyleSheet(R"(
        QMainWindow {
            background-color: #1e2126;
        }
        QTreeWidget {
            background-color: #1a1d22;
            border: none;
            padding: 5px;
        }
        QTreeWidget::item {
            padding: 3px;
            color: #c8c8c8;
        }
        QTreeWidget::item:selected {
            background-color: #3a4555;
        }
        QTreeWidget::item:hover {
            background-color: #2a3340;
        }
        QLabel {
            color: #c8c8c8;
        }
        QPushButton {
            background-color: #2a2d32;
            border: 1px solid #3a3d42;
            padding: 5px 10px;
            border-radius: 3px;
            color: #c8c8c8;
        }
        QPushButton:hover {
            background-color: #3a3d42;
        }
        QPushButton:pressed {
            background-color: #4a4d52;
        }
        QLineEdit {
            background-color: #1a1d22;
            border: 1px solid #3a3d42;
            padding: 5px;
            border-radius: 3px;
            color: #c8c8c8;
        }
        QSpinBox {
            background-color: #1a1d22;
            border: 1px solid #3a3d42;
            padding: 3px;
            border-radius: 3px;
            color: #c8c8c8;
        }
        QTabWidget::pane {
            border: 1px solid #3a3d42;
            background-color: #1a1d22;
        }
        QTabBar::tab {
            background-color: #2a2d32;
            color: #c8c8c8;
            padding: 8px 15px;
            border: 1px solid #3a3d42;
            border-bottom: none;
        }
        QTabBar::tab:selected {
            background-color: #1a1d22;
        }
        QToolBar {
            background-color: #25282d;
            border: none;
            spacing: 3px;
            padding: 2px;
        }
        QMenuBar {
            background-color: #25282d;
            color: #c8c8c8;
            padding: 3px;
        }
        QMenuBar::item {
            padding: 5px 10px;
            background-color: transparent;
        }
        QMenuBar::item:selected {
            background-color: #3a3d42;
            border-radius: 3px;
        }
        QMenu {
            background-color: #25282d;
            color: #c8c8c8;
            border: 1px solid #3a3d42;
            padding: 5px;
        }
        QMenu::item {
            padding: 5px 25px 5px 20px;
            border-radius: 3px;
        }
        QMenu::item:selected {
            background-color: #3a3d42;
        }
        QMenu::separator {
            height: 1px;
            background-color: #3a3d42;
            margin: 5px 10px;
        }
        QGroupBox {
            border: 1px solid #3a3d42;
            border-radius: 5px;
            margin-top: 10px;
            padding-top: 10px;
            color: #c8c8c8;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
        }
        QSplitter::handle {
            background-color: #2a2d32;
            width: 2px;
        }
    )");
}

QWidget* PulseEngineMainWindow::create_logo_widget()
{
    /* Create the logo widget in the top left corner */
    QWidget *logo_widget = new QWidget();
    QHBoxLayout *logo_layout = new QHBoxLayout(logo_widget);
    logo_layout->setContentsMargins(10, 5, 10, 5);
    logo_layout->setSpacing(10);

    // #TODO: Replace with actual logo icon
    QLabel *logo_label = new QLabel("#TODO");
    logo_label->setStyleSheet(R"(
        color: #5ab9ff;
        font-weight: bold;
        font-size: 14px;
        background-color: #2a2d32;
        padding: 5px 10px;
        border-radius: 5px;
    )");

    QLabel *title_label = new QLabel("Pulse Engine");
    title_label->setStyleSheet(R"(
        color: #5ab9ff;
        font-weight: bold;
        font-size: 16px;
    )");

    logo_layout->addWidget(logo_label);
    logo_layout->addWidget(title_label);
    logo_layout->addStretch();

    return logo_widget;
}

void PulseEngineMainWindow::create_menu_bar()
{
    /* Create the top menu bar */
    QMenuBar *menubar = menuBar();

    // Disable native menu bar for cross-platform consistency (especially macOS)
    menubar->setNativeMenuBar(false);

    // Add logo widget to the left of the menu bar
    QWidget *logo_widget = create_logo_widget();
    menubar->setCornerWidget(logo_widget, Qt::TopLeftCorner);

    // File menu - Project and scene operations
    QMenu *file_menu = menubar->addMenu("File");

    QAction *new_project_action = new QAction("New Project", this);
    connect(new_project_action, &QAction::triggered, this, &PulseEngineMainWindow::on_new_project); // #TODO: Implement new project creation
    file_menu->addAction(new_project_action);

    QAction *new_scene_action = new QAction("New Scene", this);
    connect(new_scene_action, &QAction::triggered, this, &PulseEngineMainWindow::on_new_scene); // #TODO: Implement new scene creation
    file_menu->addAction(new_scene_action);

    file_menu->addSeparator();

    QAction *open_project_action = new QAction("Open Project", this);
    connect(open_project_action, &QAction::triggered, this, &PulseEngineMainWindow::on_open_project); // #TODO: Implement project opening dialog
    file_menu->addAction(open_project_action);

    QAction *open_scene_action = new QAction("Open Scene", this);
    connect(open_scene_action, &QAction::triggered, this, &PulseEngineMainWindow::on_open_scene); // #TODO: Implement scene opening dialog
    file_menu->addAction(open_scene_action);

    file_menu->addSeparator();

    QAction *save_action = new QAction("Save", this);
    save_action->setShortcut(QKeySequence("Ctrl+S"));
    connect(save_action, &QAction::triggered, this, &PulseEngineMainWindow::on_save); // #TODO: Implement save current scene/project
    file_menu->addAction(save_action);

    QAction *save_as_action = new QAction("Save As...", this);
    save_as_action->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(save_as_action, &QAction::triggered, this, &PulseEngineMainWindow::on_save_as); // #TODO: Implement save as dialog
    file_menu->addAction(save_as_action);

    file_menu->addSeparator();

    QAction *export_action = new QAction("Export", this);
    connect(export_action, &QAction::triggered, this, &PulseEngineMainWindow::on_export); // #TODO: Implement export functionality
    file_menu->addAction(export_action);

    QAction *import_action = new QAction("Import", this);
    connect(import_action, &QAction::triggered, this, &PulseEngineMainWindow::on_import); // #TODO: Implement import functionality
    file_menu->addAction(import_action);

    file_menu->addSeparator();

    QAction *close_project_action = new QAction("Close Project", this);
    connect(close_project_action, &QAction::triggered, this, &PulseEngineMainWindow::on_close_project); // #TODO: Implement close project
    file_menu->addAction(close_project_action);

    QAction *exit_action = new QAction("Exit", this);
    exit_action->setShortcut(QKeySequence("Ctrl+Q"));
    connect(exit_action, &QAction::triggered, this, &PulseEngineMainWindow::on_exit);
    file_menu->addAction(exit_action);

    // Edit menu - Editing objects and scenes
    QMenu *edit_menu = menubar->addMenu("Edit");

    QAction *undo_action = new QAction("Undo", this);
    undo_action->setShortcut(QKeySequence("Ctrl+Z"));
    connect(undo_action, &QAction::triggered, this, &PulseEngineMainWindow::on_undo); // #TODO: Implement undo functionality
    edit_menu->addAction(undo_action);

    QAction *redo_action = new QAction("Redo", this);
    redo_action->setShortcut(QKeySequence("Ctrl+Y"));
    connect(redo_action, &QAction::triggered, this, &PulseEngineMainWindow::on_redo); // #TODO: Implement redo functionality
    edit_menu->addAction(redo_action);

    edit_menu->addSeparator();

    QAction *cut_action = new QAction("Cut", this);
    cut_action->setShortcut(QKeySequence("Ctrl+X"));
    connect(cut_action, &QAction::triggered, this, &PulseEngineMainWindow::on_cut); // #TODO: Implement cut selected objects
    edit_menu->addAction(cut_action);

    QAction *copy_action = new QAction("Copy", this);
    copy_action->setShortcut(QKeySequence("Ctrl+C"));
    connect(copy_action, &QAction::triggered, this, &PulseEngineMainWindow::on_copy); // #TODO: Implement copy selected objects
    edit_menu->addAction(copy_action);

    QAction *paste_action = new QAction("Paste", this);
    paste_action->setShortcut(QKeySequence("Ctrl+V"));
    connect(paste_action, &QAction::triggered, this, &PulseEngineMainWindow::on_paste); // #TODO: Implement paste objects
    edit_menu->addAction(paste_action);

    QAction *delete_action = new QAction("Delete", this);
    delete_action->setShortcut(QKeySequence("Delete"));
    connect(delete_action, &QAction::triggered, this, &PulseEngineMainWindow::on_delete); // #TODO: Implement delete selected objects
    edit_menu->addAction(delete_action);

    edit_menu->addSeparator();

    QAction *duplicate_action = new QAction("Duplicate", this);
    duplicate_action->setShortcut(QKeySequence("Ctrl+D"));
    connect(duplicate_action, &QAction::triggered, this, &PulseEngineMainWindow::on_duplicate); // #TODO: Implement duplicate selected objects
    edit_menu->addAction(duplicate_action);

    edit_menu->addSeparator();

    QAction *select_all_action = new QAction("Select All", this);
    select_all_action->setShortcut(QKeySequence("Ctrl+A"));
    connect(select_all_action, &QAction::triggered, this, &PulseEngineMainWindow::on_select_all); // #TODO: Implement select all objects
    edit_menu->addAction(select_all_action);

    QAction *invert_selection_action = new QAction("Invert Selection", this);
    invert_selection_action->setShortcut(QKeySequence("Ctrl+I"));
    connect(invert_selection_action, &QAction::triggered, this, &PulseEngineMainWindow::on_invert_selection); // #TODO: Implement invert selection
    edit_menu->addAction(invert_selection_action);

    edit_menu->addSeparator();

    QAction *preferences_action = new QAction("Preferences", this);
    preferences_action->setShortcut(QKeySequence("Ctrl+,"));
    connect(preferences_action, &QAction::triggered, this, &PulseEngineMainWindow::on_preferences); // #TODO: Implement preferences dialog
    edit_menu->addAction(preferences_action);

    QAction *settings_action = new QAction("Settings", this);
    connect(settings_action, &QAction::triggered, this, &PulseEngineMainWindow::on_edit_settings); // #TODO: Implement settings dialog
    edit_menu->addAction(settings_action);

    // Settings menu - Project and editor settings
    QMenu *settings_menu = menubar->addMenu("Settings");

    QAction *project_settings_action = new QAction("Project Settings", this);
    connect(project_settings_action, &QAction::triggered, this, &PulseEngineMainWindow::on_project_settings); // #TODO: Implement project settings dialog
    settings_menu->addAction(project_settings_action);

    QAction *editor_settings_action = new QAction("Editor Settings", this);
    connect(editor_settings_action, &QAction::triggered, this, &PulseEngineMainWindow::on_editor_settings); // #TODO: Implement editor settings dialog
    settings_menu->addAction(editor_settings_action);

    settings_menu->addSeparator();

    QAction *graphics_settings_action = new QAction("Graphics Settings", this);
    connect(graphics_settings_action, &QAction::triggered, this, &PulseEngineMainWindow::on_graphics_settings); // #TODO: Implement graphics settings
    settings_menu->addAction(graphics_settings_action);

    QAction *rendering_settings_action = new QAction("Rendering Settings", this);
    connect(rendering_settings_action, &QAction::triggered, this, &PulseEngineMainWindow::on_rendering_settings); // #TODO: Implement rendering settings
    settings_menu->addAction(rendering_settings_action);

    settings_menu->addSeparator();

    QAction *input_settings_action = new QAction("Input Settings", this);
    connect(input_settings_action, &QAction::triggered, this, &PulseEngineMainWindow::on_input_settings); // #TODO: Implement input settings
    settings_menu->addAction(input_settings_action);

    QAction *controls_settings_action = new QAction("Controls", this);
    connect(controls_settings_action, &QAction::triggered, this, &PulseEngineMainWindow::on_controls_settings); // #TODO: Implement controls configuration
    settings_menu->addAction(controls_settings_action);

    // Tools menu - Utility tools
    QMenu *tools_menu = menubar->addMenu("Tools");

    QAction *script_editor_action = new QAction("Script Editor", this);
    connect(script_editor_action, &QAction::triggered, this, &PulseEngineMainWindow::on_script_editor); // #TODO: Implement script editor
    tools_menu->addAction(script_editor_action);

    QAction *debugger_action = new QAction("Debugger", this);
    connect(debugger_action, &QAction::triggered, this, &PulseEngineMainWindow::on_debugger); // #TODO: Implement debugger
    tools_menu->addAction(debugger_action);

    tools_menu->addSeparator();

    QAction *asset_manager_action = new QAction("Asset Manager", this);
    connect(asset_manager_action, &QAction::triggered, this, &PulseEngineMainWindow::on_asset_manager); // #TODO: Implement asset manager
    tools_menu->addAction(asset_manager_action);

    tools_menu->addSeparator();

    QAction *physics_tools_action = new QAction("Physics Tools", this);
    connect(physics_tools_action, &QAction::triggered, this, &PulseEngineMainWindow::on_physics_tools); // #TODO: Implement physics tools
    tools_menu->addAction(physics_tools_action);

    QAction *ai_tools_action = new QAction("AI Tools", this);
    connect(ai_tools_action, &QAction::triggered, this, &PulseEngineMainWindow::on_ai_tools); // #TODO: Implement AI tools
    tools_menu->addAction(ai_tools_action);

    QAction *pathfinding_tools_action = new QAction("Pathfinding Tools", this);
    connect(pathfinding_tools_action, &QAction::triggered, this, &PulseEngineMainWindow::on_pathfinding_tools); // #TODO: Implement pathfinding tools
    tools_menu->addAction(pathfinding_tools_action);

    tools_menu->addSeparator();

    QAction *console_action = new QAction("Console", this);
    connect(console_action, &QAction::triggered, this, &PulseEngineMainWindow::on_console); // #TODO: Implement console window
    tools_menu->addAction(console_action);

    QAction *log_viewer_action = new QAction("Log Viewer", this);
    connect(log_viewer_action, &QAction::triggered, this, &PulseEngineMainWindow::on_log_viewer); // #TODO: Implement log viewer
    tools_menu->addAction(log_viewer_action);

    // View menu - Interface and scene visualization
    QMenu *view_menu = menubar->addMenu("View");

    // Toggle Panels submenu
    QMenu *toggle_panels_menu = view_menu->addMenu("Toggle Panels");

    toggle_hierarchy_action = new QAction("Hierarchy", this);
    toggle_hierarchy_action->setCheckable(true);
    toggle_hierarchy_action->setChecked(true);
    connect(toggle_hierarchy_action, &QAction::triggered, this, &PulseEngineMainWindow::on_toggle_hierarchy); // #TODO: Toggle hierarchy panel
    toggle_panels_menu->addAction(toggle_hierarchy_action);

    toggle_inspector_action = new QAction("Inspector", this);
    toggle_inspector_action->setCheckable(true);
    toggle_inspector_action->setChecked(true);
    connect(toggle_inspector_action, &QAction::triggered, this, &PulseEngineMainWindow::on_toggle_inspector); // #TODO: Toggle inspector panel
    toggle_panels_menu->addAction(toggle_inspector_action);

    toggle_assets_action = new QAction("Assets", this);
    toggle_assets_action->setCheckable(true);
    toggle_assets_action->setChecked(true);
    connect(toggle_assets_action, &QAction::triggered, this, &PulseEngineMainWindow::on_toggle_assets); // #TODO: Toggle assets panel
    toggle_panels_menu->addAction(toggle_assets_action);

    toggle_console_action = new QAction("Console", this);
    toggle_console_action->setCheckable(true);
    toggle_console_action->setChecked(true);
    connect(toggle_console_action, &QAction::triggered, this, &PulseEngineMainWindow::on_toggle_console); // #TODO: Toggle console panel
    toggle_panels_menu->addAction(toggle_console_action);

    toggle_explorer_action = new QAction("Explorer", this);
    toggle_explorer_action->setCheckable(true);
    toggle_explorer_action->setChecked(true);
    connect(toggle_explorer_action, &QAction::triggered, this, &PulseEngineMainWindow::on_toggle_explorer); // #TODO: Toggle explorer panel
    toggle_panels_menu->addAction(toggle_explorer_action);

    view_menu->addSeparator();

    fullscreen_action = new QAction("Fullscreen", this);
    fullscreen_action->setShortcut(QKeySequence("F11"));
    fullscreen_action->setCheckable(true);
    connect(fullscreen_action, &QAction::triggered, this, &PulseEngineMainWindow::on_toggle_fullscreen); // #TODO: Implement fullscreen toggle
    view_menu->addAction(fullscreen_action);

    // Layouts submenu
    QMenu *layouts_menu = view_menu->addMenu("Layouts");

    QAction *default_layout_action = new QAction("Default Layout", this);
    connect(default_layout_action, &QAction::triggered, this, &PulseEngineMainWindow::on_default_layout); // #TODO: Load default layout
    layouts_menu->addAction(default_layout_action);

    QAction *coding_layout_action = new QAction("Coding Layout", this);
    connect(coding_layout_action, &QAction::triggered, this, &PulseEngineMainWindow::on_coding_layout); // #TODO: Load coding layout
    layouts_menu->addAction(coding_layout_action);

    QAction *art_layout_action = new QAction("Art Layout", this);
    connect(art_layout_action, &QAction::triggered, this, &PulseEngineMainWindow::on_art_layout); // #TODO: Load art layout
    layouts_menu->addAction(art_layout_action);

    QAction *design_layout_action = new QAction("Design Layout", this);
    connect(design_layout_action, &QAction::triggered, this, &PulseEngineMainWindow::on_design_layout); // #TODO: Load design layout
    layouts_menu->addAction(design_layout_action);

    layouts_menu->addSeparator();

    QAction *save_layout_action = new QAction("Save Current Layout", this);
    connect(save_layout_action, &QAction::triggered, this, &PulseEngineMainWindow::on_save_layout); // #TODO: Save current layout
    layouts_menu->addAction(save_layout_action);

    view_menu->addSeparator();

    // Zoom submenu
    QMenu *zoom_menu = view_menu->addMenu("Zoom");

    QAction *zoom_in_action = new QAction("Zoom In", this);
    zoom_in_action->setShortcut(QKeySequence("Ctrl++"));
    connect(zoom_in_action, &QAction::triggered, this, &PulseEngineMainWindow::on_zoom_in); // #TODO: Zoom in scene view
    zoom_menu->addAction(zoom_in_action);

    QAction *zoom_out_action = new QAction("Zoom Out", this);
    zoom_out_action->setShortcut(QKeySequence("Ctrl+-"));
    connect(zoom_out_action, &QAction::triggered, this, &PulseEngineMainWindow::on_zoom_out); // #TODO: Zoom out scene view
    zoom_menu->addAction(zoom_out_action);

    QAction *zoom_reset_action = new QAction("Reset Zoom", this);
    zoom_reset_action->setShortcut(QKeySequence("Ctrl+0"));
    connect(zoom_reset_action, &QAction::triggered, this, &PulseEngineMainWindow::on_zoom_reset); // #TODO: Reset zoom to default
    zoom_menu->addAction(zoom_reset_action);

    zoom_menu->addSeparator();

    QAction *frame_selected_action = new QAction("Frame Selected", this);
    frame_selected_action->setShortcut(QKeySequence("F"));
    connect(frame_selected_action, &QAction::triggered, this, &PulseEngineMainWindow::on_frame_selected); // #TODO: Frame selected object in view
    zoom_menu->addAction(frame_selected_action);

    view_menu->addSeparator();

    show_grid_action = new QAction("Show Grid", this);
    show_grid_action->setCheckable(true);
    show_grid_action->setChecked(true);
    connect(show_grid_action, &QAction::triggered, this, &PulseEngineMainWindow::on_toggle_grid); // #TODO: Toggle grid visibility
    view_menu->addAction(show_grid_action);

    show_gizmos_action = new QAction("Show Gizmos", this);
    show_gizmos_action->setCheckable(true);
    show_gizmos_action->setChecked(true);
    connect(show_gizmos_action, &QAction::triggered, this, &PulseEngineMainWindow::on_toggle_gizmos); // #TODO: Toggle gizmos visibility
    view_menu->addAction(show_gizmos_action);

    view_menu->addSeparator();

    // Camera submenu
    QMenu *camera_menu = view_menu->addMenu("Camera");

    perspective_camera_action = new QAction("Perspective", this);
    perspective_camera_action->setCheckable(true);
    perspective_camera_action->setChecked(true);
    connect(perspective_camera_action, &QAction::triggered, this, &PulseEngineMainWindow::on_perspective_camera); // #TODO: Switch to perspective camera
    camera_menu->addAction(perspective_camera_action);

    orthographic_camera_action = new QAction("Orthographic", this);
    orthographic_camera_action->setCheckable(true);
    connect(orthographic_camera_action, &QAction::triggered, this, &PulseEngineMainWindow::on_orthographic_camera); // #TODO: Switch to orthographic camera
    camera_menu->addAction(orthographic_camera_action);

    camera_menu->addSeparator();

    QAction *top_view_action = new QAction("Top View", this);
    top_view_action->setShortcut(QKeySequence("Numpad 7"));
    connect(top_view_action, &QAction::triggered, this, &PulseEngineMainWindow::on_top_view); // #TODO: Switch to top view
    camera_menu->addAction(top_view_action);

    QAction *front_view_action = new QAction("Front View", this);
    front_view_action->setShortcut(QKeySequence("Numpad 1"));
    connect(front_view_action, &QAction::triggered, this, &PulseEngineMainWindow::on_front_view); // #TODO: Switch to front view
    camera_menu->addAction(front_view_action);

    QAction *side_view_action = new QAction("Side View", this);
    side_view_action->setShortcut(QKeySequence("Numpad 3"));
    connect(side_view_action, &QAction::triggered, this, &PulseEngineMainWindow::on_side_view); // #TODO: Switch to side view
    camera_menu->addAction(side_view_action);
}

// ===========================================
// MENU ACTION SLOTS - File Menu
// ===========================================

void PulseEngineMainWindow::on_new_project()
{
    /* #TODO: Implement new project creation dialog */
    qDebug() << "File -> New Project clicked";
}

void PulseEngineMainWindow::on_new_scene()
{
    /* #TODO: Implement new scene creation */
    qDebug() << "File -> New Scene clicked";
}

void PulseEngineMainWindow::on_open_project()
{
    /* #TODO: Implement project opening dialog */
    qDebug() << "File -> Open Project clicked";
}

void PulseEngineMainWindow::on_open_scene()
{
    /* #TODO: Implement scene opening dialog */
    qDebug() << "File -> Open Scene clicked";
}

void PulseEngineMainWindow::on_save()
{
    /* #TODO: Implement save current scene/project */
    qDebug() << "File -> Save clicked";
}

void PulseEngineMainWindow::on_save_as()
{
    /* #TODO: Implement save as dialog */
    qDebug() << "File -> Save As clicked";
}

void PulseEngineMainWindow::on_export()
{
    /* #TODO: Implement export functionality */
    qDebug() << "File -> Export clicked";
}

void PulseEngineMainWindow::on_import()
{
    /* #TODO: Implement import functionality */
    qDebug() << "File -> Import clicked";
}

void PulseEngineMainWindow::on_close_project()
{
    /* #TODO: Implement close project with confirmation */
    qDebug() << "File -> Close Project clicked";
}

void PulseEngineMainWindow::on_exit()
{
    /* Exit application with confirmation */
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit Pulse Engine", "Are you sure you want to exit?",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        close();
    }
}

// ===========================================
// MENU ACTION SLOTS - Edit Menu
// ===========================================

void PulseEngineMainWindow::on_undo()
{
    /* #TODO: Implement undo functionality */
    qDebug() << "Edit -> Undo clicked";
}

void PulseEngineMainWindow::on_redo()
{
    /* #TODO: Implement redo functionality */
    qDebug() << "Edit -> Redo clicked";
}

void PulseEngineMainWindow::on_cut()
{
    /* #TODO: Implement cut selected objects */
    qDebug() << "Edit -> Cut clicked";
}

void PulseEngineMainWindow::on_copy()
{
    /* #TODO: Implement copy selected objects */
    qDebug() << "Edit -> Copy clicked";
}

void PulseEngineMainWindow::on_paste()
{
    /* #TODO: Implement paste objects */
    qDebug() << "Edit -> Paste clicked";
}

void PulseEngineMainWindow::on_delete()
{
    /* #TODO: Implement delete selected objects */
    qDebug() << "Edit -> Delete clicked";
}

void PulseEngineMainWindow::on_duplicate()
{
    /* #TODO: Implement duplicate selected objects */
    qDebug() << "Edit -> Duplicate clicked";
}

void PulseEngineMainWindow::on_select_all()
{
    /* #TODO: Implement select all objects */
    qDebug() << "Edit -> Select All clicked";
}

void PulseEngineMainWindow::on_invert_selection()
{
    /* #TODO: Implement invert selection */
    qDebug() << "Edit -> Invert Selection clicked";
}

void PulseEngineMainWindow::on_preferences()
{
    /* #TODO: Implement preferences dialog */
    qDebug() << "Edit -> Preferences clicked";
}

void PulseEngineMainWindow::on_edit_settings()
{
    /* #TODO: Implement settings dialog */
    qDebug() << "Edit -> Settings clicked";
}

// ===========================================
// MENU ACTION SLOTS - Settings Menu
// ===========================================

void PulseEngineMainWindow::on_project_settings()
{
    /* #TODO: Implement project settings dialog */
    qDebug() << "Settings -> Project Settings clicked";
}

void PulseEngineMainWindow::on_editor_settings()
{
    /* #TODO: Implement editor settings dialog */
    qDebug() << "Settings -> Editor Settings clicked";
}

void PulseEngineMainWindow::on_graphics_settings()
{
    /* #TODO: Implement graphics settings */
    qDebug() << "Settings -> Graphics Settings clicked";
}

void PulseEngineMainWindow::on_rendering_settings()
{
    /* #TODO: Implement rendering settings */
    qDebug() << "Settings -> Rendering Settings clicked";
}

void PulseEngineMainWindow::on_input_settings()
{
    /* #TODO: Implement input settings */
    qDebug() << "Settings -> Input Settings clicked";
}

void PulseEngineMainWindow::on_controls_settings()
{
    /* #TODO: Implement controls configuration */
    qDebug() << "Settings -> Controls clicked";
}

// ===========================================
// MENU ACTION SLOTS - Tools Menu
// ===========================================

void PulseEngineMainWindow::on_script_editor()
{
    /* #TODO: Implement script editor */
    qDebug() << "Tools -> Script Editor clicked";
}

void PulseEngineMainWindow::on_debugger()
{
    /* #TODO: Implement debugger */
    qDebug() << "Tools -> Debugger clicked";
}

void PulseEngineMainWindow::on_asset_manager()
{
    /* #TODO: Implement asset manager */
    qDebug() << "Tools -> Asset Manager clicked";
}

void PulseEngineMainWindow::on_physics_tools()
{
    /* #TODO: Implement physics tools */
    qDebug() << "Tools -> Physics Tools clicked";
}

void PulseEngineMainWindow::on_ai_tools()
{
    /* #TODO: Implement AI tools */
    qDebug() << "Tools -> AI Tools clicked";
}

void PulseEngineMainWindow::on_pathfinding_tools()
{
    /* #TODO: Implement pathfinding tools */
    qDebug() << "Tools -> Pathfinding Tools clicked";
}

void PulseEngineMainWindow::on_console()
{
    /* #TODO: Implement console window */
    qDebug() << "Tools -> Console clicked";
}

void PulseEngineMainWindow::on_log_viewer()
{
    /* #TODO: Implement log viewer */
    qDebug() << "Tools -> Log Viewer clicked";
}

// ===========================================
// MENU ACTION SLOTS - View Menu
// ===========================================

void PulseEngineMainWindow::on_toggle_hierarchy()
{
    /* #TODO: Toggle hierarchy panel visibility */
    qDebug() << "View -> Toggle Hierarchy:" << toggle_hierarchy_action->isChecked();
}

void PulseEngineMainWindow::on_toggle_inspector()
{
    /* #TODO: Toggle inspector panel visibility */
    qDebug() << "View -> Toggle Inspector:" << toggle_inspector_action->isChecked();
}

void PulseEngineMainWindow::on_toggle_assets()
{
    /* #TODO: Toggle assets panel visibility */
    qDebug() << "View -> Toggle Assets:" << toggle_assets_action->isChecked();
}

void PulseEngineMainWindow::on_toggle_console()
{
    /* #TODO: Toggle console panel visibility */
    qDebug() << "View -> Toggle Console:" << toggle_console_action->isChecked();
}

void PulseEngineMainWindow::on_toggle_explorer()
{
    /* #TODO: Toggle explorer panel visibility */
    qDebug() << "View -> Toggle Explorer:" << toggle_explorer_action->isChecked();
}

void PulseEngineMainWindow::on_toggle_fullscreen()
{
    /* #TODO: Implement fullscreen toggle */
    if (fullscreen_action->isChecked()) {
        showFullScreen();
        qDebug() << "View -> Fullscreen: ON";
    } else {
        showNormal();
        qDebug() << "View -> Fullscreen: OFF";
    }
}

void PulseEngineMainWindow::on_default_layout()
{
    /* #TODO: Load default layout */
    qDebug() << "View -> Layouts -> Default Layout clicked";
}

void PulseEngineMainWindow::on_coding_layout()
{
    /* #TODO: Load coding layout */
    qDebug() << "View -> Layouts -> Coding Layout clicked";
}

void PulseEngineMainWindow::on_art_layout()
{
    /* #TODO: Load art layout */
    qDebug() << "View -> Layouts -> Art Layout clicked";
}

void PulseEngineMainWindow::on_design_layout()
{
    /* #TODO: Load design layout */
    qDebug() << "View -> Layouts -> Design Layout clicked";
}

void PulseEngineMainWindow::on_save_layout()
{
    /* #TODO: Save current layout */
    qDebug() << "View -> Layouts -> Save Current Layout clicked";
}

void PulseEngineMainWindow::on_zoom_in()
{
    /* #TODO: Zoom in scene view */
    qDebug() << "View -> Zoom In clicked";
}

void PulseEngineMainWindow::on_zoom_out()
{
    /* #TODO: Zoom out scene view */
    qDebug() << "View -> Zoom Out clicked";
}

void PulseEngineMainWindow::on_zoom_reset()
{
    /* #TODO: Reset zoom to default */
    qDebug() << "View -> Reset Zoom clicked";
}

void PulseEngineMainWindow::on_frame_selected()
{
    /* #TODO: Frame selected object in view */
    qDebug() << "View -> Frame Selected clicked";
}

void PulseEngineMainWindow::on_toggle_grid()
{
    /* #TODO: Toggle grid visibility */
    qDebug() << "View -> Show Grid:" << show_grid_action->isChecked();
}

void PulseEngineMainWindow::on_toggle_gizmos()
{
    /* #TODO: Toggle gizmos visibility */
    qDebug() << "View -> Show Gizmos:" << show_gizmos_action->isChecked();
}

void PulseEngineMainWindow::on_perspective_camera()
{
    /* #TODO: Switch to perspective camera */
    perspective_camera_action->setChecked(true);
    orthographic_camera_action->setChecked(false);
    qDebug() << "View -> Camera -> Perspective";
}

void PulseEngineMainWindow::on_orthographic_camera()
{
    /* #TODO: Switch to orthographic camera */
    orthographic_camera_action->setChecked(true);
    perspective_camera_action->setChecked(false);
    qDebug() << "View -> Camera -> Orthographic";
}

void PulseEngineMainWindow::on_top_view()
{
    /* #TODO: Switch to top view */
    qDebug() << "View -> Camera -> Top View clicked";
}

void PulseEngineMainWindow::on_front_view()
{
    /* #TODO: Switch to front view */
    qDebug() << "View -> Camera -> Front View clicked";
}

void PulseEngineMainWindow::on_side_view()
{
    /* #TODO: Switch to side view */
    qDebug() << "View -> Camera -> Side View clicked";
}

// ===========================================
// TOOLBAR ACTIONS
// ===========================================

void PulseEngineMainWindow::on_toolbar_edit()
{
    /* #TODO: Implement edit mode */
    qDebug() << "Toolbar -> Edit clicked";
}

void PulseEngineMainWindow::on_toolbar_search()
{
    /* #TODO: Implement search functionality */
    qDebug() << "Toolbar -> Search clicked";
}

void PulseEngineMainWindow::on_toolbar_play()
{
    /* #TODO: Implement play/run scene */
    qDebug() << "Toolbar -> Play clicked";
}

void PulseEngineMainWindow::on_toolbar_settings()
{
    /* #TODO: Open settings */
    qDebug() << "Toolbar -> Settings clicked";
}

void PulseEngineMainWindow::on_toolbar_tools()
{
    /* #TODO: Open tools menu */
    qDebug() << "Toolbar -> Tools clicked";
}

// ===========================================
// SCENE VIEW TOOLBAR ACTIONS
// ===========================================

void PulseEngineMainWindow::on_scene_edit()
{
    /* #TODO: Implement scene edit mode */
    qDebug() << "Scene Toolbar -> Edit clicked";
}

void PulseEngineMainWindow::on_scene_zoom()
{
    /* #TODO: Implement scene zoom tool */
    qDebug() << "Scene Toolbar -> Zoom clicked";
}

void PulseEngineMainWindow::on_scene_filter()
{
    /* #TODO: Implement scene filter */
    qDebug() << "Scene Toolbar -> Filter clicked";
}

void PulseEngineMainWindow::on_scene_settings()
{
    /* #TODO: Implement scene settings */
    qDebug() << "Scene Toolbar -> Settings clicked";
}

void PulseEngineMainWindow::on_scene_transform()
{
    /* #TODO: Implement transform tool */
    qDebug() << "Scene Toolbar -> Transform clicked";
}

void PulseEngineMainWindow::on_scene_view()
{
    /* #TODO: Switch to scene view */
    qDebug() << "Scene Toolbar -> Scene View clicked";
}

void PulseEngineMainWindow::on_2d_view()
{
    /* #TODO: Switch to 2D view */
    qDebug() << "Scene Toolbar -> 2D clicked";
}

void PulseEngineMainWindow::on_view_options()
{
    /* #TODO: Open view options */
    qDebug() << "Scene Toolbar -> Options clicked";
}

void PulseEngineMainWindow::on_move_tool()
{
    /* #TODO: Activate move tool */
    qDebug() << "Scene Toolbar -> Move clicked";
}

void PulseEngineMainWindow::on_rotate_tool()
{
    /* #TODO: Activate rotate tool */
    qDebug() << "Scene Toolbar -> Rotate clicked";
}

void PulseEngineMainWindow::on_scale_tool()
{
    /* #TODO: Activate scale tool */
    qDebug() << "Scene Toolbar -> Scale clicked";
}

void PulseEngineMainWindow::on_grid_tool()
{
    /* #TODO: Toggle grid tool */
    qDebug() << "Scene Toolbar -> Grid clicked";
}

void PulseEngineMainWindow::on_snap_tool()
{
    /* #TODO: Toggle snap tool */
    qDebug() << "Scene Toolbar -> Snap clicked";
}

void PulseEngineMainWindow::on_camera_tool()
{
    /* #TODO: Camera tool options */
    qDebug() << "Scene Toolbar -> Camera clicked";
}

void PulseEngineMainWindow::on_render_tool()
{
    /* #TODO: Render options */
    qDebug() << "Scene Toolbar -> Render clicked";
}

void PulseEngineMainWindow::on_view_tool()
{
    /* #TODO: View tool options */
    qDebug() << "Scene Toolbar -> View clicked";
}

// ===========================================
// EXPLORER TOOLBAR ACTIONS
// ===========================================

void PulseEngineMainWindow::on_explorer_copy()
{
    /* #TODO: Copy selected asset */
    qDebug() << "Explorer -> Copy clicked";
}

void PulseEngineMainWindow::on_explorer_open()
{
    /* #TODO: Open selected asset */
    qDebug() << "Explorer -> Open clicked";
}

void PulseEngineMainWindow::on_explorer_back()
{
    /* #TODO: Navigate back in explorer */
    qDebug() << "Explorer -> Back clicked";
}

void PulseEngineMainWindow::on_explorer_refresh()
{
    /* #TODO: Refresh explorer view */
    qDebug() << "Explorer -> Refresh clicked";
}

void PulseEngineMainWindow::on_explorer_filter()
{
    /* #TODO: Filter explorer assets */
    qDebug() << "Explorer -> Filter clicked";
}

void PulseEngineMainWindow::on_explorer_clear()
{
    /* #TODO: Clear selection */
    qDebug() << "Explorer -> Clear clicked";
}

void PulseEngineMainWindow::on_explorer_preview()
{
    /* #TODO: Toggle preview mode */
    qDebug() << "Explorer -> Preview clicked";
}

void PulseEngineMainWindow::on_explorer_delete()
{
    /* #TODO: Delete selected asset */
    qDebug() << "Explorer -> Delete clicked";
}

void PulseEngineMainWindow::on_explorer_show()
{
    /* #TODO: Show asset options */
    qDebug() << "Explorer -> Show clicked";
}

void PulseEngineMainWindow::on_explorer_menu()
{
    /* #TODO: Open context menu */
    qDebug() << "Explorer -> Menu clicked";
}

void PulseEngineMainWindow::on_explorer_dropdown()
{
    /* #TODO: Open dropdown menu */
    qDebug() << "Explorer -> Dropdown clicked";
}

void PulseEngineMainWindow::on_explorer_audio()
{
    /* #TODO: Audio tools */
    qDebug() << "Explorer -> Audio clicked";
}

void PulseEngineMainWindow::on_explorer_display()
{
    /* #TODO: Display options */
    qDebug() << "Explorer -> Display clicked";
}