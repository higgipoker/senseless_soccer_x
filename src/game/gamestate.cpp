#include "gamestate.h"
#include <SFML/Window/Event.hpp>
namespace ss {
Gamestate::Gamestate (sf::RenderWindow& wnd) : window (wnd) {
}

void Gamestate::frame() {

    //
    // HANDLE INPUT
    //
    if (window.hasFocus()) {
        sf::Event event;
        while (window.pollEvent (event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else {
                handle_input (event);
            }
        }
    }

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

}
