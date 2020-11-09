#include "match.h"
#include "../menu/button.h"

#include <iostream>
#include <memory>

namespace ss {
namespace game {

void Match::handle_input (const sf::Event& evt) {
    for (auto& controller : controllers) {
        controller.update();
    }
    for (auto& controllable : controllables) {
        controllable.update();
    }
}

void Match::update() {
    for (auto& movable : movables) {
        movable.update();
    }
}

void Match::draw() {
    for (auto& renderable : renderables) {
        renderable.update();
    }
}

}// namespace game
}// namespace ss
