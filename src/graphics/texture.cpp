#include "texture.h"

namespace ss {
namespace graphics {

using namespace sf;

void swapColors (Texture* tex, const std::vector<std::pair<sf::Color, sf::Color> >& palette) {
    sf::Image img = tex->copyToImage();
    for (auto& colours : palette) {
        for (unsigned int x = 0; x < img.getSize().x; ++x) {
            for (unsigned int y = 0; y < img.getSize().y; ++y) {
                if (img.getPixel (x, y) == colours.first) {
                    img.setPixel (x, y, colours.second);
                }
            }
        }
    }
    tex->loadFromImage (img);
}

} // namespace
}// namespace
