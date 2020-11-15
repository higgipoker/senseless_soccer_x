#pragma once
#include "../graphics/Sprite.hpp"
#include <SFML/Graphics/RenderTexture.hpp>
#include <vector>

namespace ss {
namespace game {
using KitPalette = std::vector<std::pair<sf::Color, sf::Color>>;


class MatchResources {
public:
    static void init();
    static KitPalette makeStandardRedKit();
    static KitPalette makeStandardBlueKit();
    static sf::RenderTexture match_texture;
    static Sprite match_sprite;
private:
    static bool inited;
};
}
}
