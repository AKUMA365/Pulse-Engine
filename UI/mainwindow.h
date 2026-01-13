#ifndef PULSEENGINEMAINWINDOW_H
#define PULSEENGINEMAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QSplitter>
#include <QTreeWidget>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPalette>
#include <QColor>
#include <QMenuBar>
#include <QMenu>
#include <QApplication>
#include <QDebug>

class PulseEngineMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    PulseEngineMainWindow(QWidget *parent = nullptr);
    ~PulseEngineMainWindow();

private:
    void setup_dark_theme();
    void create_menu_bar();
    QWidget* create_left_panel();
    QSplitter* create_center_panel();
    QWidget* create_logo_widget();

    // Member actions that need to be accessed in slots
    QAction *toggle_hierarchy_action;
    QAction *toggle_inspector_action;
    QAction *toggle_assets_action;
    QAction *toggle_console_action;
    QAction *toggle_explorer_action;
    QAction *fullscreen_action;
    QAction *show_grid_action;
    QAction *show_gizmos_action;
    QAction *perspective_camera_action;
    QAction *orthographic_camera_action;

private slots:
    // File Menu
    void on_new_project();
    void on_new_scene();
    void on_open_project();
    void on_open_scene();
    void on_save();
    void on_save_as();
    void on_export();
    void on_import();
    void on_close_project();
    void on_exit();

    // Edit Menu
    void on_undo();
    void on_redo();
    void on_cut();
    void on_copy();
    void on_paste();
    void on_delete();
    void on_duplicate();
    void on_select_all();
    void on_invert_selection();
    void on_preferences();
    void on_edit_settings();

    // Settings Menu
    void on_project_settings();
    void on_editor_settings();
    void on_graphics_settings();
    void on_rendering_settings();
    void on_input_settings();
    void on_controls_settings();

    // Tools Menu
    void on_script_editor();
    void on_debugger();
    void on_asset_manager();
    void on_physics_tools();
    void on_ai_tools();
    void on_pathfinding_tools();
    void on_console();
    void on_log_viewer();

    // View Menu
    void on_toggle_hierarchy();
    void on_toggle_inspector();
    void on_toggle_assets();
    void on_toggle_console();
    void on_toggle_explorer();
    void on_toggle_fullscreen();
    void on_default_layout();
    void on_coding_layout();
    void on_art_layout();
    void on_design_layout();
    void on_save_layout();
    void on_zoom_in();
    void on_zoom_out();
    void on_zoom_reset();
    void on_frame_selected();
    void on_toggle_grid();
    void on_toggle_gizmos();
    void on_perspective_camera();
    void on_orthographic_camera();
    void on_top_view();
    void on_front_view();
    void on_side_view();

    // Toolbar Actions
    void on_toolbar_edit();
    void on_toolbar_search();
    void on_toolbar_play();
    void on_toolbar_settings();
    void on_toolbar_tools();

    // Scene View Toolbar Actions
    void on_scene_edit();
    void on_scene_zoom();
    void on_scene_filter();
    void on_scene_settings();
    void on_scene_transform();
    void on_scene_view();
    void on_2d_view();
    void on_view_options();
    void on_move_tool();
    void on_rotate_tool();
    void on_scale_tool();
    void on_grid_tool();
    void on_snap_tool();
    void on_camera_tool();
    void on_render_tool();
    void on_view_tool();

    // Explorer Toolbar Actions
    void on_explorer_copy();
    void on_explorer_open();
    void on_explorer_back();
    void on_explorer_refresh();
    void on_explorer_filter();
    void on_explorer_clear();
    void on_explorer_preview();
    void on_explorer_delete();
    void on_explorer_show();
    void on_explorer_menu();
    void on_explorer_dropdown();
    void on_explorer_audio();
    void on_explorer_display();
};

#endif // PULSEENGINEMAINWINDOW_H