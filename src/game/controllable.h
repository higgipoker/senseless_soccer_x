#pragma once

namespace ss {
namespace game {

class Entity;
class Controllable {
public:
void update(){};
private:
    Entity* entity = nullptr;
};
}// namespace game
}// namespace ss
