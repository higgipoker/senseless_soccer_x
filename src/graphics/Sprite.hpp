#pragma once

#include <SFML/Graphics/Sprite.hpp>

namespace ss {

const int MAX_FRAMES = 32;

class Sprite : public sf::Sprite {
public:
    int z = 0;
    void setFrame(const int frame);

protected:
    std::array<sf::IntRect, MAX_FRAMES> frames;
};

}// namespace senseless

