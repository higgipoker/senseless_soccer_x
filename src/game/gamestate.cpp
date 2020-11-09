#include "gamestate.h"
#include <SFML/Window/Event.hpp>
namespace ss {
namespace game {
    
Gamestate::Gamestate (sf::RenderWindow& wnd) : window (wnd) {
}

void Gamestate::frame() {

    //
    // HANDLE INPUT
    //
    handle_input();

    //
    // UPDATE WORLD
    update();

    //
    // DRAW SCREEN
    //
    window.clear (clear_color);
    draw();
    window.display();
}

}// namepsace game
}// namespace ss
