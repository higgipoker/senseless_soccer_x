#pragma once
#include "../game/Gamestate.h"
#include "Page.h"
#include "Animation.h"
#include "../input/input.h"
#include <memory>

namespace ss {
namespace game {
    
constexpr int DefaultPage = 0;
class Menu : public Gamestate {
public:
    Menu (sf::RenderWindow& wnd);
    ~Menu();
    void start() override;
    void end() override;
    void addwidget (std::unique_ptr<Widget> w, int page = DefaultPage);
protected:
    void handle_input () override;
    void update() override;
    void draw() override;

private:
    std::vector<std::unique_ptr<Page>> pages;
    std::vector<Animation*> animations;
};

} // namespace game
} // namepsace ss