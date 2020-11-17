#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <array>

namespace ss {
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
const int MAX_FRAMES = 32;

class Sprite : public sf::Sprite {
public:
    void init(const SpriteDefinition& def);
    void setFrame (const int frame);
    int z = 0;

protected:
    std::array<sf::IntRect, MAX_FRAMES> frames;
};

}// namespace senseless

