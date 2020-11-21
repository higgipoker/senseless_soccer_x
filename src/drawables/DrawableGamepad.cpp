#include "DrawableGamepad.hpp"
#include "../resources/Resources.hpp"

namespace ss {
namespace drawables {

DrawableGamepad::DrawableGamepad() {
    left_stick.init ({LEFT_STICK_X, LEFT_STICK_Y});
    right_stick.init ({RIGHT_STICK_X, RIGHT_STICK_Y});

    bg.setSize ({512, 344});
    bg_texture.loadFromFile (resources::gfx_folder() + "gamepad_bg.png");
    bg.setTexture (&bg_texture);
    bg.setTextureRect ({0, 0, 512, 344});

    thumbstick_texture.loadFromFile (resources::gfx_folder() + "thumbstick.png");
    left_stick.origin.setTexture (&thumbstick_texture);
    right_stick.origin.setTexture (&thumbstick_texture);
    left_stick.origin.setTextureRect({0, 0, 64, 64});
    right_stick.origin.setTextureRect({0, 0, 64, 64});

}

void DrawableGamepad::draw (sf::RenderTarget& target, sf::RenderStates states) const {
    if (controller_state == nullptr) {
        return;
    }
    target.draw (bg);
    draw_analog_stick (left_stick, controller_state->left_stick_vector, target, states);
    draw_analog_stick (right_stick, controller_state->right_stick_vector, target, states);
}

void DrawableGamepad::setPosition (const sf::Vector2f& pos) {
    transformable.setPosition (pos);
    bg.setPosition (pos);
    left_stick.setPosition (pos);
    right_stick.setPosition (pos);
}

void DrawableGamepad::update() {
    left_stick.update (controller_state->left_stick_vector);
    right_stick.update (controller_state->right_stick_vector);

}

void DrawableGamepad::draw_analog_stick (const AnalogStick& stick, const sf::Vector3f& stick_vector, sf::RenderTarget& target, sf::RenderStates states) const {

    target.draw (stick.outer);
//    target.draw (stick.inner);
    target.draw (stick.origin);
//    target.draw (stick.line);
}


}// namespace drawables
}// namespace ss
