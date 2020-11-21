#include "PlayerStateRun.hpp"
#include "Player.hpp"
#include "player_animations.h"
#include "../math/vector.hpp"

namespace ss {
namespace game {

PlayerStateRun::PlayerStateRun (Player& p) : PlayerState (p) {
}

void PlayerStateRun::start() {
    player.sprite->setActiveAnimation (player_animations::ID_RUN);
}

void PlayerStateRun::stop() {

}

void PlayerStateRun::update() {
    PlayerState::update();
}

bool PlayerStateRun::finished() {
    return (equal(vec_magnitude2d (player.velocity),  0));
}

void PlayerStateRun::changeState() {
    player.current_state = &player.stand;
}

}// namespace game
}// namespace ss
