#ifndef SFMLWIDGET_H
#define SFMLWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QResizeEvent>
#include <SFML/Graphics.hpp>
#include "../scene/scene.h"

class SFMLWidget : public QWidget, public sf::RenderWindow {
    Q_OBJECT

public:
    explicit SFMLWidget(QWidget* parent = nullptr);
    virtual ~SFMLWidget() = default;

    scene& GetScene() { return m_scene; }

    signals:
        void entitySelected(size_t entityID);

protected:
    void showEvent(QShowEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    QPaintEngine* paintEngine() const override;

private:
    QTimer m_timer;
    scene m_scene;
    bool m_initialized = false;

    sf::View m_editorView;
    sf::Vector2i m_lastMousePos;
    bool m_isPanning = false;
    float m_zoomLevel = 1.0f;

private slots:
    void onFrameUpdate();
};

#endif