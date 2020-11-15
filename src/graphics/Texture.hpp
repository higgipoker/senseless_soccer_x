#pragma once
#include <SFML/Graphics/Texture.hpp>
namespace ss {
class Texture : public sf::Texture {
public:
    void swapColors(const std::vector<std::pair<sf::Color, sf::Color> >& palette);
};
}
