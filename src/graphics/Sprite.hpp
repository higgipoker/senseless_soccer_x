#pragma once
#include "SpriteAnimation.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <array>

namespace ss {
namespace sprite {
static const int MAX_FRAMES = 32;
static const int MAX_ANIMATIONS = 10;
// ********************
// Sprite Definition
// ********************
struct SpriteDefinition {
    const sf::Texture*      texture          = nullptr;
    sf::Vector2f            texture_origin;
    int                     total_frames     = 0;
    int                     frame_width      = 0;
    int                     frame_height     = 0;
    int                     spritesheet_cols = 0;
};
// ********************
// Sprit 
// ********************
struct Sprite {
    sf::Sprite sprite;
    int z = 0;
    std::array<sf::IntRect,     MAX_FRAMES>     frames;
    std::array<SpriteAnimation, MAX_ANIMATIONS> animations;
    SpriteAnimation*                            current_animation = nullptr;
};
// ********************
// Functions 
// ********************
void    init            (Sprite* sprite, const SpriteDefinition* def);
void    set_frame       (Sprite* sprite, const int frame);
void    init_animation  (Sprite* sprite, const int id, const AnimationDefinition* anim_def);
void    set_animation   (Sprite* sprite, const int id);
void    animate         (Sprite* sprite);

} // namepsace sprite
}// namespace senseless

