// #include "PlayerStateStand.hpp"
// #include "Player.hpp"
// #include "player_animations.h"
// #include "../graphics/Sprite.hpp"
// #include "../math/vector.hpp"
// 
// namespace ss {
// namespace game {
// 
// PlayerStateStand::PlayerStateStand (Player& p) : PlayerState (p) {
// 
// }
// 
// void PlayerStateStand::start() {
//     PlayerState::start();
//     player.heading = vec_normalized (player.heading);
//     player.sprite->sprite.setRotation (vec_angle (vec_normalized ({player.heading.x, -player.heading.y, 0})) - 90);
//     player.sprite->sprite.setActiveAnimation (player_animations::ID_STAND);
// }
// 
// void PlayerStateStand::stop() {
//     PlayerState::stop();
// }
// 
// void PlayerStateStand::update() {
//     PlayerState::update();
// 
// }
// 
// bool PlayerStateStand::finished() {
//     return (greater_than(vec_magnitude2d (player.velocity), 0)); 
// }
// 
// void PlayerStateStand::changeState() {
//     player.current_state = &player.run;
// }
// 
// }
// }
