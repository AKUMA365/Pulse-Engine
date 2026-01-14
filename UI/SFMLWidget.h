#ifndef SFMLWIDGET_H
#define SFMLWIDGET_H

#include <QWidget>
#include <QTimer>
#include <SFML/Graphics.hpp>
#include "../scene/scene.h" // Подключаем вашу сцену

class SFMLWidget : public QWidget, public sf::RenderWindow {
    Q_OBJECT

public:
    explicit SFMLWidget(QWidget* parent = nullptr);
    virtual ~SFMLWidget() = default;

private:
    void showEvent(QShowEvent*) override;
    void paintEvent(QPaintEvent*) override;
    QPaintEngine* paintEngine() const override;

    QTimer m_timer;

    scene m_scene;
    bool m_initialized = false;

private slots:
    void onFrameUpdate();
};

#endif // SFMLWIDGET_H