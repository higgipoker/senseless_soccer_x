#include "global.hpp"
#include "files/files.h"
#include "game/Gamestate.hpp"
#include "menu/menu.cpp"
#include "input/Gamepad.hpp"
#include "game/Match.hpp"
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
    Controller gamepad; // defaults to a gamepad tyle controller
    //Menu menu (window, gamepad);
    Menu main_menu;
    Match match (window);
    window.setFramerateLimit (60);
    
    
    // test file handling
    files::File file;
    files::open(file, files::working_dir() +"/data/calibration.cfg");
    files::read_lines(file);
    
    std::map<int, calibration::Calibration> c;
    
    calibration::load_from_file(c, file);
    
    
    files::close(file);
    

    while (window.isOpen()) {

        int return_code = run_menu (&main_menu, &window);

        if (return_code == 0) {
            window.close();

        } else if (return_code == 2) {
            match.init();
            match.play();
            match.exit();
            wait_for_no_key (window);
        }
    }

    global::log ("Senseless Soccer finished");
    return 0;
}
