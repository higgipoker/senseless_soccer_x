#include "Player.hpp"
#include "../math/vector.hpp"

namespace ss {
namespace game {
void Player::update() {
  if(sprite){
   sprite->setRotation(vec_angle(heading));
   std::cout << vec_angle(heading) << std::endl;
  }
}
}// namespace game
}// namespace ss
