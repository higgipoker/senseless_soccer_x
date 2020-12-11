#pragma once
#include "engine.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
namespace ss {
namespace match {

    struct Resources {
        sf::RenderTexture match_texture;
    };
   
    enum State      {Prematch, Lineup, Kickoff, Play, Goalkick, Throwin, Corner, Penalty, Postmatch, Finished};
    struct Match {
        Resources   resources;
        State       state       = Prematch;
    };
    void init_match           (Match* match);
    void init_match_resources (Resources* resources);
    void frame                (Match* match, engine::MatchEngine* engine, sf::RenderWindow* window);

} // namespace
} // namespace


// #pragma once
// #include "MatchState.hpp"
// #include "MatchPlay.hpp"
// #include "Engine.hpp"
// #include "match_resources.hpp"
//
// namespace ss {
// namespace game {
// class Match {
// public:
//     Match (sf::RenderWindow& wnd);
//     void init();
//     void play();
//     void exit();
//
//     sf::RenderTexture* getMatchTexture(){
//        return &MatchResources::match_texture;
//     }
// private:
//     Engine engine;
//     bool finished = false;
//     MatchPlay playstate;
//     MatchState* current_state = &playstate;
//
//     void update();
// public:
//     friend class MatchPlay;
// };
// }// namespace
// }
