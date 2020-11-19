#pragma once
#include "SpriteAnimation.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <array>

namespace ss {
static const int MAX_FRAMES = 32;
static const int MAX_ANIMATIONS = 10;

struct SpriteDefinition {
    SpriteDefinition(const sf::Texture& tex) : texture(tex){
        
    }
    const sf::Texture& texture;
    sf::Vector2f texture_origin;
    int total_frames = 0;
    int frame_width = 0;
    int frame_height = 0;
    int spritesheet_cols = 0;
};

class Sprite : public sf::Sprite {
public:
    void init(const SpriteDefinition& def);
    void setFrame (const int frame);
    void configAnimation(const int id, const AnimationDefinition& anim_def);
    void setActiveAnimation(const int id);
    void animate();
    int z = 0;

protected:
    std::array<sf::IntRect, MAX_FRAMES>         frames;
    std::array<SpriteAnimation, MAX_ANIMATIONS> animations;
    SpriteAnimation* current_animation = nullptr;

};

}// namespace senseless

