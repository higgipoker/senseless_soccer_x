#include "game/Gamestate.hpp"
#include "menu/Menu.hpp"
#include "input/Gamepad.hpp"
#include "game/Match.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace ss;
using namespace game;

void wait_for_no_key (sf::RenderWindow& window) {
    static sf::Event event;
    while (window.pollEvent (event));
}

int main() {
    std::cout << "Senseless soccer started" << std::endl;
    sf::RenderWindow window (sf::VideoMode{1280, 720}, "Senseless Soccer");
    Gamepad gamepad;
    Menu menu (window, gamepad);
    Match match (window);
    window.setFramerateLimit (60);
    while (window.isOpen()) {
        switch (menu.run()) {
        case MenuEvent::Exit:
            window.close();
            break;
        case MenuEvent::Friendly:
            match.init();
            match.play();
            match.exit();
            wait_for_no_key(window);
            break;
        default:
            break;

        }
    }
//     while (window.isOpen()) {
//         switch (menu.frame()) {
//         case MenuEvent::Exit:
//             window.close();
//             break;
//         case MenuEvent::Friendly:
//             match.init();
//             match.play();
//             match.exit();
//             wait_for_no_key (window);
//             break;
//         case MenuEvent::Settings:
//             menu.changePage (Page_ID::Calibrate);
//             break;
//         case MenuEvent::None:
//         default:
//             break;
//         }
//     }
    std::cout << "Senseless Soccer finished" << std::endl;
    return 0;
}
