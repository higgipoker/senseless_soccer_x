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
    
    static SpriteDefinition getPlayerSpriteDef(int which = 0);
private:
    static bool inited;
    
    static sf::Vector2f player1_origin, player2_origin;
};
} // namespace game
} // namespace ss
