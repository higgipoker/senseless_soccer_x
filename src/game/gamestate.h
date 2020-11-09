#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>

namespace ss {
namespace game {
enum class State{Main_Menu, Match};
class Gamestate {
public:
    Gamestate (sf::RenderWindow& wnd);
    virtual void start() = 0;
    virtual void frame() final;
    virtual void end() = 0;
    
    virtual bool stateOver(){return false;}
    virtual State nextState(){return State::Main_Menu;}

protected:
    virtual void handle_input () = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    sf::RenderWindow& window;
    sf::Color clear_color{sf::Color::Black};
    
    std::string name = "unset";
};
} // namespace game
} // namespace ss
