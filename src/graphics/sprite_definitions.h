#pragma once
#include "Sprite.hpp"

namespace ss {
namespace graphics {
inline sprite::SpriteDefinition make_player_sprite_definition (const sf::Texture* tex, const sf::Vector2f origin) {
    return sprite::SpriteDefinition {
        tex,
        origin,
        14,
        32, 32,
        7
    };
}

inline sprite::SpriteDefinition make_ball_sprite_definition (const sf::Texture* tex, const sf::Vector2f origin) {
    return sprite::SpriteDefinition {
        tex,
        origin,
        8,
        32, 32,
        8
    };
}

inline void make_run_animation (sprite::SpriteAnimation* anim) {

    anim->frames[0] = 7;
    anim->frames[1] = 8;
    anim->frames[2] = 9;
    anim->frames[3] = 10;
    anim->frames[4] = 11;
    anim->frames[5] = 12;
    anim->frames[6] = 13;
    anim->number_frames = 7;
    anim->loop = true;
    anim->running = true;
    anim->ticks_per_frame = 10;
}

inline void make_stand_animation (sprite::SpriteAnimation* anim) {
    anim->frames[0] = 0;
    anim->number_frames = 1;
    anim->loop = true;
    anim->running = true;
}

inline void make_roll_animation (sprite::SpriteAnimation* anim) {

    anim->frames[0] = 0;
    anim->frames[1] = 1;
    anim->frames[2] = 2;
    anim->frames[3] = 3;
    anim->frames[4] = 4;
    anim->frames[5] = 5;
    anim->frames[6] = 6;
    anim->number_frames = 7;
    anim->loop = true;
    anim->running = true;
    anim->ticks_per_frame = 10;
}

} // namespace
} // namespace
