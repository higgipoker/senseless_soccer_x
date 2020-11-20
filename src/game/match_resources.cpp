#include "match_resources.hpp"
#include "../graphics/Texture.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>
#include <filesystem>

namespace ss {
namespace game {

bool MatchResources::inited = false;
Sprite MatchResources::match_sprite;
sf::RenderTexture MatchResources::match_texture;
sf::Vector2f MatchResources::player1_origin;
sf::Vector2f MatchResources::player2_origin;

void MatchResources::init() {
    if (inited) return;
    inited = true;
    static const std::string player_spritesheet_filename = "player.png";
    static const std::string player_shadow_spritesheet_filename = "player_shadow.png";
    static const std::string grass_filename = "grass_dry.png";
    static const std::string ball_filename = "ball.png";
    static const std::string lines_filename = "pitch_lines.png";

    std::filesystem::path path (std::filesystem::current_path());
    std::string gfx_path = path.string() + "/gfx/";
    // home team players
    Texture home_team_texture;
    home_team_texture.loadFromFile (gfx_path + player_spritesheet_filename);
    home_team_texture.swapColors (makeStandardRedKit());

    // away team players
    Texture away_team_texture;
    away_team_texture.loadFromFile (gfx_path + player_spritesheet_filename);
    away_team_texture.swapColors (makeStandardBlueKit());

    // player shadows
    Texture shadows_texture;
    shadows_texture.loadFromFile (gfx_path + player_shadow_spritesheet_filename);

    // ball
    Texture ball_texture;
    ball_texture.loadFromFile (gfx_path + ball_filename);

    // grass
    Texture grass_texture;
    grass_texture.loadFromFile (gfx_path + grass_filename);

    // pitch lines
    Texture lines_texture;
    lines_texture.loadFromFile (gfx_path + lines_filename);

    // find the width of the mega texture
    auto width = std::max ({    home_team_texture.getSize().x,
                                away_team_texture.getSize().x,
                                shadows_texture.getSize().x,
                                ball_texture.getSize().x,
                                grass_texture.getSize().x,
                                lines_texture.getSize().x
                           });

    // find the height of the mega texture
    auto height =   home_team_texture.getSize().y +
                    away_team_texture.getSize().y +
                    shadows_texture.getSize().y +
                    ball_texture.getSize().y +
                    grass_texture.getSize().y +
                    lines_texture.getSize().y;

    // create the mega texture with the necessary dimensions
    if (!match_texture.create (width, height)) {
        std::cout << "Could not create tecture with dimensions " << width << "x" << height << std::endl;
        std::cout << "Max texture dimensions: " << sf::Texture::getMaximumSize() << " x " << sf::Texture::getMaximumSize() << std::endl;
    }

    // create sprites to draw to the mega texture
    sf::Sprite home_team_sprite (home_team_texture);
    sf::Sprite away_team_sprite (away_team_texture);
    sf::Sprite player_shadow_sprite (shadows_texture);
    sf::Sprite ball_sprite (ball_texture);
    sf::Sprite grass_sprite (grass_texture);
    sf::Sprite lines_sprite (lines_texture);
    away_team_sprite.move (0, home_team_sprite.getLocalBounds().height);
    player_shadow_sprite.move (0, away_team_sprite.getGlobalBounds().top + away_team_sprite.getLocalBounds().height);
    ball_sprite.move (0, player_shadow_sprite.getGlobalBounds().top + player_shadow_sprite.getLocalBounds().height);
    grass_sprite.move (0, ball_sprite.getGlobalBounds().top + ball_sprite.getLocalBounds().height);
    lines_sprite.move (0, grass_sprite.getGlobalBounds().top + grass_sprite.getLocalBounds().height);
    
    player1_origin = home_team_sprite.getPosition();
    player2_origin = away_team_sprite.getPosition();

    // draw stuff to the mega texture
    match_texture.clear ({0, 0, 0, 0});
    match_texture.draw (home_team_sprite);
    match_texture.draw (away_team_sprite);
    match_texture.draw (player_shadow_sprite);
    match_texture.draw (ball_sprite);
    match_texture.draw (grass_sprite);
    match_texture.draw (lines_sprite);
    match_texture.display();

    // tmp debug to display the mega texture
    match_sprite.setTexture (match_texture.getTexture());
}

KitPalette MatchResources::makeStandardRedKit() {
    return {};
}

KitPalette MatchResources::makeStandardBlueKit() {
    return {
        {sf::Color (135, 0, 0), sf::Color (0, 0, 135) },
        {sf::Color (175, 0, 0), sf::Color (0, 0, 175) },
        {sf::Color (215, 0, 0), sf::Color (0, 0, 215) },
        {sf::Color (255, 0, 0), sf::Color (0, 0, 255) }
    };
}

SpriteDefinition MatchResources::getPlayerSpriteDef(int which ){
    SpriteDefinition def(match_texture.getTexture());
    def.texture_origin = which == 0? player1_origin : player2_origin;
    def.spritesheet_cols = 7;
    def.total_frames = 16;
    def.frame_width = 64;
    def.frame_height = 64;
    return def;
}

} // namespace game
} // namespace ss
