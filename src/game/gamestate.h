#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>

namespace ss{
class Gamestate{
public:
    Gamestate(sf::RenderWindow& wnd);
    virtual void start() = 0;
    virtual void frame() final;
    virtual void end() = 0;
    
protected:
    virtual void handle_input(const sf::Event& evt) = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    
    sf::RenderWindow& window;
    sf::Color clear_color{sf::Color::Magenta};
};    
}
