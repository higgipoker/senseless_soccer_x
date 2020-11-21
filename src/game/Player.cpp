#include "Player.hpp"
#include "../math/math.hpp"
#include "../math/vector.hpp"
#include <iostream>

namespace ss {
namespace game {

Player::Player() : stand (*this), run (*this) {

}
void Player::update() {
    
    // state machine
    current_state->update();
    if (current_state->finished()) {
        current_state->stop();
        current_state->changeState();
        current_state->start();
    }
    update_position();
    if (sprite) {
        sprite->setPosition ({position.x, position.y});
        sprite->setRotation(vec_angle(sf::Vector3f{velocity.x, -velocity.y, 0}) -90);
    }
}

void Player::handleInput (const ControllerState& controller_state) {
    current_state->handleInput(controller_state);
}

void Player::rotate_towards (const sf::Vector3f& dir) {
    target_dir = dir;
}

void Player::update_position() {
    force =  force * speed;
    sf::Vector3f acceleration = force;
    sf::Vector3f friction = vec_reversed (velocity) * 0.1f;
    velocity += acceleration + friction;
    position = position + velocity;

    // damp
    static const float DAMP_VELOCITY = 0.1f;
    if (less_than (vec_magnitude2d (velocity), DAMP_VELOCITY)) vec_reset (velocity);
    vec_reset(force);
}
}// namespace game
}// namespace ss
