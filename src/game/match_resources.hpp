#pragma once
#include "../graphics/Sprite.hpp"
#include <SFML/Graphics/RenderTexture.hpp>
#include <vector>
#include <array>

namespace ss {
namespace game {
static const size_t MAX_SPRITES = 100;
using KitPalette = std::vector<std::pair<sf::Color, sf::Color>>;


class MatchResources {
public:
    static void init();
    static KitPalette makeStandardRedKit();
    static KitPalette makeStandardBlueKit();
    static sf::RenderTexture match_texture;
    static Sprite match_sprite;
    static SpriteDefinition getPlayerSpriteDef (int which = 1);
    
    int acquireSprite();
private:
    static bool inited;
    static sf::Vector2f player1_origin, player2_origin;

    std::array<Sprite, MAX_SPRITES> sprite_pool;
    int used_sprites = 0;

};
} // namespace game
} // namespace ss
