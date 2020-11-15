#pragma once
#include "../menu/Menu.hpp"
#include "../input/Gamepad.hpp"
#include "../graphics/Sprite.hpp"
#include "../graphics/TileMap.hpp"
#include "Entity.hpp"

namespace ss {
namespace game {
const size_t MAX_ENTITIES  = 100;
const size_t MAX_SPRITES = MAX_ENTITIES * 2; // each entity could have a sprite and each sprite could have a shadow
const size_t MAX_CONTROLLERS = 30;

class Engine {
public:
    explicit Engine (sf::RenderWindow& wnd);
    
    void addSprite(Sprite *sprite);
    
    void frame();
    bool paused = false;

private:
    sf::RenderWindow& window;
    TileMap pitch;
    std::array<Movable, MAX_ENTITIES> movables;
    std::array<Sprite*, MAX_SPRITES> sprites;
    std::array<Controllable, MAX_ENTITIES> controllables;
    std::array<Gamepad, MAX_CONTROLLERS> controllers;
    
    size_t number_movables = 0;
    size_t number_sprites = 0;
    size_t number_controllables = 0;
    size_t number_controllers = 0;

    void handle_input();
    void update();
    void draw();
};
}// namespace game
}// namespace ss

