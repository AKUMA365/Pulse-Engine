#include "SFMLWidget.h"
#include <QShowEvent>
#include <QPaintEvent>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

SFMLWidget::SFMLWidget(QWidget* parent) : QWidget(parent) {
    // Настраиваем атрибуты, чтобы Qt не мешал SFML рисовать
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::StrongFocus); // Чтобы виджет ловил нажатия клавиш

    // Инициализация сцены (загрузка текстур и т.д.)
    m_scene.init();

    // Настраиваем таймер на ~60 FPS (1000 мс / 60 ≈ 16 мс)
    connect(&m_timer, &QTimer::timeout, this, &SFMLWidget::onFrameUpdate);
    m_timer.start(16);
}

void SFMLWidget::showEvent(QShowEvent*) {
    if (!m_initialized) {
        // Создаем окно SFML, используя WinId (хэндл окна) от виджета Qt
        sf::RenderWindow::create(reinterpret_cast<sf::WindowHandle>(winId()));
        m_initialized = true;
    }
}

QPaintEngine* SFMLWidget::paintEngine() const {
    return nullptr;
}

void SFMLWidget::paintEvent(QPaintEvent*) {
    onFrameUpdate();
}

void SFMLWidget::onFrameUpdate() {
    if (isOpen()) {
        clear(sf::Color::Black);

        m_scene.render(*this);

        display();
    }
}