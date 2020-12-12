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

} // namespace
} // namespace
