#include "game/game.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace ss;
using namespace game;

int main() {
    std::cout << "Senseless soccer started" << std::endl;
    sf::RenderWindow window(sf::VideoMode{800, 600}, "Senseless Soccer");
    Game game(window);
    game.init();
    game.run();
    game.end();
    std::cout << "Senseless Soccer finished" << std::endl;
    return 0;
}
