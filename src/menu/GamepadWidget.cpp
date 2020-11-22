#include "GamepadWidget.hpp"

namespace ss {
GamepadWidget::GamepadWidget (sf::RenderWindow& wnd) : Widget (wnd) {
    gamepad.controller_state = &controller.state;
}

void GamepadWidget::draw_self() {
    controller.update();
    gamepad.update();
    window.draw (gamepad);
}

void GamepadWidget::setPosition (const sf::Vector2f& p) {
    gamepad.setPosition (p);
}

void GamepadWidget::setSize (const sf::Vector2f& s) {
    gamepad.setScale(s.x/gamepad.getSize().x, s.y/gamepad.getSize().y );
}
void GamepadWidget::onHighlight() {}
void GamepadWidget::onUnHighlight() {}
}
