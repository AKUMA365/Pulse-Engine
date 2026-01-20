#include "SFMLWidget.h"
#include "../EngineCore/components.h"
#include <entt/entt.hpp>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

SFMLWidget::SFMLWidget(QWidget* parent) : QWidget(parent) {
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    connect(&m_timer, &QTimer::timeout, this, &SFMLWidget::onFrameUpdate);
    m_timer.start(16);

    m_editorView.setCenter({0.f, 0.f});
    m_editorView.setSize({1600.f, 900.f});
}

void SFMLWidget::showEvent(QShowEvent*) {
    if (!m_initialized) {
        sf::RenderWindow::create(reinterpret_cast<sf::WindowHandle>(winId()));
        m_initialized = true;

        sf::Vector2u size = getSize();
        m_editorView.setSize({static_cast<float>(size.x), static_cast<float>(size.y)});
        m_editorView.setCenter({0.f, 0.f});
    }
}

QPaintEngine* SFMLWidget::paintEngine() const {
    return nullptr;
}

void SFMLWidget::paintEvent(QPaintEvent*) {
    onFrameUpdate();
}

void SFMLWidget::resizeEvent(QResizeEvent* event) {
    if (m_initialized) {
        sf::Vector2u size = getSize();
        m_editorView.setSize({static_cast<float>(size.x) * m_zoomLevel, static_cast<float>(size.y) * m_zoomLevel});
    }
    QWidget::resizeEvent(event);
}

void SFMLWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MiddleButton || (event->button() == Qt::LeftButton && event->modifiers() & Qt::AltModifier)) {
        m_isPanning = true;
        m_lastMousePos = {event->pos().x(), event->pos().y()};
        setCursor(Qt::ClosedHandCursor);
        return;
    }

    if (event->button() == Qt::LeftButton && m_initialized) {
        sf::Vector2i mousePos = {event->pos().x(), event->pos().y()};
        sf::Vector2f worldPos = mapPixelToCoords(mousePos, m_editorView);

        auto& registry = m_scene.GetRegistry();
        auto view = registry.view<TransformComponent, SpriteComponent>();

        bool found = false;
        for (auto entity : view) {
            auto& spriteComp = view.get<SpriteComponent>(entity);
            if (spriteComp.Sprite.has_value()) {
                if (spriteComp.Sprite->getGlobalBounds().contains(worldPos)) {
                    emit entitySelected((size_t)entity);
                    found = true;
                    break;
                }
            }
        }


    }
}

void SFMLWidget::mouseMoveEvent(QMouseEvent* event) {
    if (m_isPanning) {
        sf::Vector2i currentMousePos = {event->pos().x(), event->pos().y()};

        sf::Vector2f delta;
        delta.x = static_cast<float>(m_lastMousePos.x - currentMousePos.x) * m_zoomLevel;
        delta.y = static_cast<float>(m_lastMousePos.y - currentMousePos.y) * m_zoomLevel;

        m_editorView.move(delta);
        m_lastMousePos = currentMousePos;
    }
}

void SFMLWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::MiddleButton || (event->button() == Qt::LeftButton)) {
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);
    }
}

void SFMLWidget::wheelEvent(QWheelEvent* event) {
    if (event->angleDelta().y() > 0) {
        m_zoomLevel *= 0.9f;
    } else {
        m_zoomLevel *= 1.1f;
    }

    sf::Vector2u size = getSize();
    m_editorView.setSize({static_cast<float>(size.x) * m_zoomLevel, static_cast<float>(size.y) * m_zoomLevel});
}

void SFMLWidget::onFrameUpdate() {
    if (isOpen()) {
        clear(sf::Color(20, 20, 20));

        setView(m_editorView);
        m_scene.render(*this);

        display();
    }
}