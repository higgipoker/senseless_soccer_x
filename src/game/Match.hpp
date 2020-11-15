#pragma once
#include "MatchState.hpp"
#include "MatchPlay.hpp"
#include "Engine.hpp"
#include "match_resources.hpp"

namespace ss {
namespace game {
class Match {
public:
    Match (sf::RenderWindow& wnd);
    void init();
    void play();
    void exit();
    
    sf::RenderTexture* getMatchTexture(){
       return &MatchResources::match_texture; 
    }
private:
    Engine engine;
    bool finished = false;
    MatchPlay playstate;
    MatchState* current_state = &playstate;
    
    void update();
public:
    friend class MatchPlay;
};
}// namespace
}
