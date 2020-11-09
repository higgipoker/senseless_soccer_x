#pragma once
#include "../game/gamestate.h"
#include "page.h"
#include "animation.h"
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
    void handle_input (const sf::Event& event) override;
    void update() override;
    void draw() override;

private:
    std::vector<std::unique_ptr<Page>> pages;
    std::vector<Animation*> animations;
};

} // namespace game
} // namepsace ss
