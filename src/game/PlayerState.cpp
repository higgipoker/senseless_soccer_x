// #include "PlayerState.hpp"
// #include "Player.hpp"
// #include "../math/vector.hpp"
// 
// namespace ss {
// namespace game {
// PlayerState::PlayerState (Player& p) : player (p) {
// 
// }
// 
// void PlayerState::start() {
// }
// 
// void PlayerState::stop() {
// }
// 
// void PlayerState::update() {
// }
// 
// bool PlayerState::finished() {
//     return false;
// }
// 
// void PlayerState::handleInput(const ControllerState& controller_state){
//     sf::Vector3f vector{controller_state.left_stick_vector.x, controller_state.left_stick_vector.y, 0};
//     player.heading = vec_normalized (vector);
//     float stick_vector_mag = vec_magnitude2d(vector) * 2;
//     player.force += (player.heading * stick_vector_mag);
// }
// }
// }
