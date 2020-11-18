#pragma once
#include "Page.hpp"
#include "AnimationFlash.hpp"
#include "AnimationFade.hpp"
#include "../input/Gamepad.hpp"
#include <memory>

namespace ss {

constexpr int DefaultPage = 0;

class Menu : public Controllable {
public:
    Menu (sf::RenderWindow& wnd, Gamepad& controller);
    ~Menu();
    Widget* addwidget (std::unique_ptr<Widget> w, int page = DefaultPage);
    MenuEvent frame();
    void onInputEvent (const InputEvent in_event, const std::vector<int>& in_params) override;
    void waitForNoKey();

    bool mouse_pressed = false;
    bool mouse_moved = false;
    sf::Vector2f mouse_position, last_mouse_position;
    bool mouse_mode = false;

private:
    sf::RenderWindow& window;
    Gamepad& gamepad;
    MenuEvent return_code = MenuEvent::None;
    std::vector<std::unique_ptr<Page>> pages;
    AnimationFlash active_animation;
    AnimationFade page_animation;
    Page* active_page = nullptr;
    Widget* active_widget = nullptr;
    bool mouse_down = false;
    int gamepad_wait = 0;
    const int GAMEPAD_WAIT_TIME = 15; // frames
    bool fire_pressed = false;
    bool wait_for_mouse_up = false;

    // input helpers
    void update_mouse();
    void read_keyboard();
    void read_mouse();
    void read_gamepad();
    bool wait_for_gamepad() {
        if (gamepad_wait)
            return --gamepad_wait;
        return false;
    }
};

} // namepsace ss


