#include "Player.hpp"
#include "../math/vector.hpp"
#include <iostream>

namespace ss {
namespace game {
void Player::update() {
  if(sprite){
   sprite->setRotation(vec_angle(heading));
  }
}
}// namespace game
}// namespace ss
