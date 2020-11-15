#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>

namespace ss {
namespace game {

struct SerializedState {
    char footer[32];
    char header[32];
    char data [1024];
};

class Gamestate {
public:
    Gamestate (sf::RenderWindow& wnd);
    virtual void start() = 0;
    virtual void frame() final;
    virtual void end() = 0;

    virtual bool stateOver() {
        return false;
    }
    virtual SerializedState save() {
        SerializedState state;
        return state;
    }
    virtual void restore (const SerializedState state) {};

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
