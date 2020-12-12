#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <vector>

namespace ss {
namespace graphics {
void swapColors (sf::Texture* tex, const std::vector<std::pair<sf::Color, sf::Color> >& palette);
}// namespace
}// namespace
