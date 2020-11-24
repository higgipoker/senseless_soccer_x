#pragma once
#include "Button.hpp"
#include <string>
#include <memory>

namespace ss {

static inline std::unique_ptr<Widget> make_button (sf::RenderWindow& wnd, const int i,  const std::string& caption, const sf::Vector2f& pos, const sf::Color& col) {
    std::unique_ptr<Widget> btn = std::make_unique<Button> (wnd, i, caption, pos, col);
    return btn;
}

static inline sf::Color color_std {0, 120, 10};
static inline sf::Color color_ext {55, 0, 0};
}
