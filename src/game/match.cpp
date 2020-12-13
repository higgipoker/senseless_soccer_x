#include "match.h"
#include "pitch_tiles.h"
#include "../graphics/sprite_definitions.h"
#include "../global.hpp"
#include "../graphics/texture.h"
#include "../physics/collisions.h"
#include "../math/vector.hpp"
namespace ss {
namespace match {

using namespace sf;
void init_match_resources (Match* match, engine::MatchEngine* engine) {

    std::filesystem::path path (std::filesystem::current_path());
    std::string gfx_path = path.string() + "/gfx/";
    // home team players
    Texture home_team_texture;
    Texture away_team_texture;
    home_team_texture.loadFromFile (global::graphicsPath() + "player.png");
    away_team_texture.loadFromFile (global::graphicsPath() + "player.png");
    graphics::swapColors (&home_team_texture, match->teams[0].kit);
    graphics::swapColors (&away_team_texture, match->teams[1].kit);

    // player shadows
    Texture shadows_texture;
    shadows_texture.loadFromFile (global::graphicsPath() + "player_shadow.png");

    // ball
    Texture ball_texture;
    ball_texture.loadFromFile (global::graphicsPath() + "ball.png");

    // grass
    Texture grass_texture;
    grass_texture.loadFromFile (global::graphicsPath() + "grass_dry.png");

    // pitch lines
    Texture lines_texture;
    lines_texture.loadFromFile (global::graphicsPath() + "pitch_lines.png");

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
    if (!match->resources.match_texture.create (width, height)) {
        std::cout << "Could not create tecture with dimensions " << width << "x" << height;
        std::cout << "Max texture dimensions: " << sf::Texture::getMaximumSize() << " x " << sf::Texture::getMaximumSize();
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

    // draw stuff to the mega texture
    match->resources.match_texture.clear ({0, 0, 0, 0});
    match->resources.match_texture.draw (home_team_sprite);
    match->resources.match_texture.draw (away_team_sprite);
    match->resources.match_texture.draw (player_shadow_sprite);
    match->resources.match_texture.draw (ball_sprite);
    match->resources.match_texture.draw (grass_sprite);
    match->resources.match_texture.draw (lines_sprite);
    match->resources.match_texture.display();

    // tmp debug to display the mega texture
    match->match_sprite.sprite.setTexture (match->resources.match_texture.getTexture());

    // tell the engine to use the mega texture for the grass
    engine->pitch_grass.init (
        match->resources.match_texture,
        global::to_int_rect (grass_sprite.getGlobalBounds()),
        global::to_int_vector ({grass_sprite.getLocalBounds().width, grass_sprite.getLocalBounds().height}),
        grass_tiles,
        10,
        15);


    // tell the engine to use the mega texture for the pitch lines
    engine->pitch_lines.init (
        match->resources.match_texture,
        global::to_int_rect (lines_sprite.getGlobalBounds()),
        sf::Vector2u (32, 32),
        pitch_line_tiles,
        50,
        80,
        0);

    // add the players
    sprite::SpriteDefinition player_sprite_def = graphics::make_player_sprite_definition (&match->resources.match_texture.getTexture(), {home_team_sprite.getGlobalBounds().left, home_team_sprite.getGlobalBounds().top});
    for (int i = 0; i < 4; ++i) {
        int id = engine::add_player (engine);
        sprite::Sprite* sprite = &engine->sprites.drawable[id];
        sprite::init (sprite, &player_sprite_def);
        engine->sprites.movable[id].movable2.position = {300 + i * 10.f, 300 + i * 10.f};
        if (i == 0) {
            attach_controller (engine, 0, id);
            attach_to (&engine->camera, &engine->sprites.movable[id]);
        }
        init (&engine->players[id], engine);
    }

    // config the ball
    add_ball(engine);
    sprite::SpriteDefinition ball_sprite_def = graphics::make_ball_sprite_definition (&match->resources.match_texture.getTexture(), {ball_sprite.getGlobalBounds().left, ball_sprite.getGlobalBounds().top});
    auto sprite = &engine->sprites.drawable[engine->ball.id];
    sprite::init (sprite, &ball_sprite_def);
    sprite->sprite.setPosition (500, 500);
    engine->sprites.movable[engine->ball.id].movable3.position = {500, 500, 0};
//    sprite->sprite.setScale (0.4, 0.4);
    graphics::make_roll_animation (&engine->sprites.animation[engine->ball.id]);
}

void play (Match* match, engine::MatchEngine* engine, sf::RenderWindow* window) {

    //while (match->state != Finished) {
    while (true) {

        switch (match->state) {
        case Prematch:    break;
        case Lineup:      break;
        case Kickoff:     break;
        case Play:        break;
        case Goalkick:    break;
        case Throwin:     break;
        case Corner:      break;
        case Penalty:     break;
        case Postmatch:   break;
        case Finished:    break;
        }
        
        // check collisions players with ball
        for(int i=0;i<engine->used_players; ++i){
            if(physics::collides(&engine->players[i].feet, &engine->ball.collider)){
                engine->sprites.movable[engine->ball.id].movable3.applied_force = vec_to_3d(engine->sprites.movable[engine->players[i].id].movable2.velocity)*1.f;
                std::cout << "collision" << std::endl;
            }
                
        }

        engine::frame (engine, window,  0.01f);
    }
}

} // namespace
} // namespace


// #include "Match.hpp"
// #include "Player.hpp"
// #include "player_animations.h"
// #include "../graphics/Sprite.hpp"
// #include "../global.hpp"
// #include <SFML/Window/Keyboard.hpp>
// namespace ss {
// namespace game {
//
// Player* add_player (Engine& engine) {
//     int id = engine.addPlayer();
//     Player* player = engine.getPlayer (id);
//     player->id = id;
//     player->sprite = engine.getSprite (player->id);
//     player->sprite->setPosition ({100, 100});
//
//     player->sprite->configAnimation (player_animations::ID_STAND, player_animations::animation_stand());
//     player->sprite->configAnimation (player_animations::ID_RUN, player_animations::animation_run());
//     player->sprite->setActiveAnimation (player_animations::ID_STAND);
//     int cid = engine.addController();
// //    engine.getController(cid)->type = ControllerType::Keyboard;
//     engine.attachController(cid, id);
//     return player;
// }
//
// Match::Match (sf::RenderWindow& wnd) : engine (wnd) {
//     MatchResources::init();
//     Player* player1 = add_player (engine);
//     global::log("add player with id: " + std::to_string(player1->id));
// }
//
// void Match::init() {
//     // set up the players, match parameters etc here
//     finished = false;
// }
//
// void Match::exit() {
//     // do any cleanup, reporting etc here
// }
//
// void Match::play() {
//     while (!finished) {
//         // TODO debug
//         if (sf::Keyboard::isKeyPressed (sf::Keyboard::Escape)) {
//             finished = true;
//         }
//         update();
//         if (engine.paused) {
//             // do pause menu here
//         } else {
//             engine.frame();
//         }
//     }
// }
//
// void Match::update() {
//     current_state->step();
//     if (current_state->finished()) {
//         current_state->stop();
//         current_state->changeState (*this);
//         current_state->start();
//     }
// }
// }// namespace game
// }// namespace ss


