#pragma once

namespace ss {
namespace game {
class Entity;
class Renderable {
public:
    void update(){};
private:
    Entity* entity = nullptr;
};
}// anmespace game
} // namespace ss
