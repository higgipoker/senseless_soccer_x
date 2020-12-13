#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <array>
#include "SpriteAnimation.hpp"

namespace ss {
namespace sprite {
static const int MAX_FRAMES = 32;
static const int MAX_ANIMATIONS = 10;
static const int MAX_ANIMATION_FRAMES = 8;
// ********************
// Sprite Definition
// ********************
struct SpriteDefinition {
    const sf::Texture*      texture          {nullptr};
    sf::Vector2f            texture_origin   {0,0};
    int                     total_frames     {0};
    int                     frame_width      {0};
    int                     frame_height     {0};
    int                     spritesheet_cols {0};
};
// ********************
// Animation Definition
// ********************
struct AnimationDefinition {
    bool loop = false;
    int number_frames = 0;
    int frames_per_frame = 0;
    int frames[MAX_ANIMATION_FRAMES]; 
};

// ********************
// SpriteAnimation 
// ********************
struct SpriteAnimation {
  int  frames          [MAX_ANIMATION_FRAMES];
  int  number_frames   {0};
  int  current_frame   {0};
  int  act_frame       {0};
  bool loop            {true};
  int  ticks_per_frame {0};
  int  current_ticks   {0};
  bool running         {false};
};
// ********************
// Sprite 
// ********************
struct Sprite {
    sf::Sprite      sprite;
    int             z                 {0};
    int             current_animation {0};
    sf::IntRect     frames            [MAX_FRAMES];
};
// ********************
// Functions 
// ********************
void    init            (Sprite* sprite, const SpriteDefinition* def);
void    set_frame       (Sprite* sprite, const int frame);
void    init_animation  (SpriteAnimation* animation, const AnimationDefinition* anim_def);
void    set_animation   (Sprite* sprite, const int id);
void    animate         (SpriteAnimation* anim);

} // namepsace sprite
}// namespace senseless

