#pragma once
#include "Controllable.hpp"
#include "Movable.hpp"
#include "../menu/Menu.hpp"
#include "../input/Gamepad.hpp"
#include "../graphics/Sprite.hpp"
#include "../graphics/SpriteAnimation.hpp"
#include "../graphics/TileMap.hpp"
#include <cassert>

namespace ss {
namespace game {
const size_t MAX_ENTITIES  = 100;
const size_t MAX_SPRITES = MAX_ENTITIES * 2; // each entity could have a sprite and each sprite could have a shadow
const size_t MAX_CONTROLLERS = 30;

class Engine {
public:
    explicit Engine (sf::RenderWindow& wnd);

    int addSprite (const SpriteDefinition& def);
    inline Sprite& getSPrite (int id) {
        assert (static_cast<size_t> (id) < MAX_SPRITES);
        return sprites[id];
    }
    inline void setAnimation( const AnimationDefinition& anim_def){
        animations[anim_def.entity_id].init(anim_def);
        animations[anim_def.entity_id].start();
    }

    void frame();
    bool paused = false;

private:
    sf::RenderWindow& window;
    TileMap pitch;
    std::array<Movable, MAX_ENTITIES>           movables;
    std::array<Sprite, MAX_SPRITES>             sprites;
    std::array<SpriteAnimation, MAX_SPRITES>    animations;
    std::array<Controllable, MAX_ENTITIES>      controllables;
    std::array<Gamepad, MAX_CONTROLLERS>        controllers;

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


