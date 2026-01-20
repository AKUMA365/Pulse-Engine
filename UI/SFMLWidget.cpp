#include "SFMLWidget.h"
#include "../EngineCore/components.h"
#include <cmath>

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

    m_zoomLevel = 1.0f;

    m_editorView.setCenter({0.f, 0.f});
    m_editorView.setSize({1600.f, 900.f});
}

void SFMLWidget::showEvent(QShowEvent*) {
    if (!m_initialized) {
        sf::RenderWindow::create(reinterpret_cast<sf::WindowHandle>(winId()));
        m_initialized = true;

        sf::Vector2u size = getSize();
        m_editorView.setSize({static_cast<float>(size.x) * m_zoomLevel, static_cast<float>(size.y) * m_zoomLevel});
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
    if (event->angleDelta().y() == 0) return;

    float zoomFactor = (event->angleDelta().y() > 0) ? 0.9f : 1.1f;

    float newZoom = m_zoomLevel * zoomFactor;

    if (newZoom < 0.1f) newZoom = 0.1f;
    if (newZoom > 10.0f) newZoom = 10.0f;

    m_zoomLevel = newZoom;

    sf::Vector2u size = getSize();
    m_editorView.setSize({static_cast<float>(size.x) * m_zoomLevel, static_cast<float>(size.y) * m_zoomLevel});
}

void SFMLWidget::drawGrid() {
    float baseGridSize = 100.0f;
    if (m_zoomLevel > 2.0f) baseGridSize = 500.0f;
    if (m_zoomLevel < 0.5f) baseGridSize = 50.0f;

    sf::Color gridColor(60, 60, 60);
    sf::View view = getView();

    sf::Vector2f center = view.getCenter();
    sf::Vector2f size = view.getSize();

    float left = center.x - size.x / 2.0f;
    float right = center.x + size.x / 2.0f;
    float top = center.y - size.y / 2.0f;
    float bottom = center.y + size.y / 2.0f;

    int gridSize = static_cast<int>(baseGridSize);

    int startX = static_cast<int>(std::floor(left / gridSize)) * gridSize;
    int endX = static_cast<int>(std::ceil(right / gridSize)) * gridSize;
    int startY = static_cast<int>(std::floor(top / gridSize)) * gridSize;
    int endY = static_cast<int>(std::ceil(bottom / gridSize)) * gridSize;

    std::vector<sf::Vertex> lines;
    lines.reserve((endX - startX + endY - startY) / gridSize * 4);

    for (int x = startX; x <= endX; x += gridSize) {
        lines.push_back(sf::Vertex(sf::Vector2f((float)x, top), gridColor));
        lines.push_back(sf::Vertex(sf::Vector2f((float)x, bottom), gridColor));
    }

    for (int y = startY; y <= endY; y += gridSize) {
        lines.push_back(sf::Vertex(sf::Vector2f(left, (float)y), gridColor));
        lines.push_back(sf::Vertex(sf::Vector2f(right, (float)y), gridColor));
    }

    if (top <= 0 && bottom >= 0) {
        lines.push_back(sf::Vertex(sf::Vector2f(left, 0), sf::Color(200, 80, 80)));
        lines.push_back(sf::Vertex(sf::Vector2f(right, 0), sf::Color(200, 80, 80)));
    }
    if (left <= 0 && right >= 0) {
        lines.push_back(sf::Vertex(sf::Vector2f(0, top), sf::Color(80, 200, 80)));
        lines.push_back(sf::Vertex(sf::Vector2f(0, bottom), sf::Color(80, 200, 80)));
    }

    if (!lines.empty()) {
        draw(lines.data(), lines.size(), sf::PrimitiveType::Lines);
    }
}

void SFMLWidget::onFrameUpdate() {
    if (isOpen()) {
        float dt = m_deltaClock.restart().asSeconds();

        m_scene.update(dt);

        clear(sf::Color(30, 30, 30));

        setView(m_editorView);

        drawGrid();
        m_scene.render(*this);

        display();
    }
}