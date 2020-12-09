#include "global.hpp"
#include "files/files.h"
#include "game/Gamestate.hpp"
#include "menu/menu.cpp"
#include "input/Gamepad.hpp"
#include "game/Match.hpp"
#include "game/engine.h"
#include "input/controller_tools.hpp"
#include <SFML/Graphics.hpp>

using namespace ss;
using namespace game;
using namespace menu;

void wait_for_no_key (sf::RenderWindow& window) {
    static sf::Event event;
    while (window.pollEvent (event));
}

int main() {
    global::log ("Senseless soccer started");
    global::Resources::load();
    sf::RenderWindow window (sf::VideoMode{1280, 720}, "Senseless Soccer");
    Controller gamepad; // defaults to a gamepad style controller
    //Menu menu (window, gamepad);
    Menu main_menu;
    window.setFramerateLimit (60);
    
    // test engine
    engine::MatchEngine* engine = new engine::MatchEngine();
    
    while (window.isOpen()) {
        engine::frame(engine, &window, 0.01f);

        int return_code = run_menu (&main_menu, &window);

        if (return_code == 0) {
            window.close();

        } else if (return_code == 2) {
           
            wait_for_no_key (window);
        }
    }
    delete engine;
    global::log ("Senseless Soccer finished");
    return 0;
}
