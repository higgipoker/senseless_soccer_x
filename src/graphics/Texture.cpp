#include "Texture.hpp"

namespace ss {
void Texture::swapColors (const std::vector<std::pair<sf::Color, sf::Color> >& palette) {
    sf::Image img = copyToImage();
    for (auto& colours : palette) {
        for (unsigned int x = 0; x < img.getSize().x; ++x) {
            for (unsigned int y = 0; y < img.getSize().y; ++y) {
                if (img.getPixel (x, y) == colours.first) {
                    img.setPixel (x, y, colours.second);
                }
            }
        }
    }
    loadFromImage (img);
}
}
