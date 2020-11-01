#include "game/game.h"
#include <iostream>
using namespace ss;

int main() {
    std::cout << "Senseless soccer started" << std::endl;
    Game game;
    game.init();
    game.run();
    game.end();
    std::cout << "Senseless Soccer finished" << std::endl;
    return 0;
}
