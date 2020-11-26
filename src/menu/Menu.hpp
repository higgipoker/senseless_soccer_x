#pragma once
#include "PageMain.hpp"
#include "PageCalibrate.hpp"
#include "AnimationFlash.hpp"
#include "AnimationFade.hpp"
#include "../input/Controller.hpp"

#include <memory>

namespace ss {

enum struct Page_ID {Main, Calibrate};

class Menu {
public:
    Menu (sf::RenderWindow& wnd, Controller& controller);
    ~Menu();
    int run();
    void changePage (const Page_ID id);

    bool mouse_pressed = false;
    bool mouse_moved = false;
    sf::Vector2f mouse_position, last_mouse_position;
    bool mouse_mode = false;
    bool gamepad_enabled = true;

private:
     
    bool exit = false;
    sf::RenderWindow& window;
    Controller& gamepad;
    int return_code = 0;
    PageMain mainpage;
    PageCalibrate calibratepage;
    AnimationFlash active_animation;
    AnimationFade page_animation;
    Page* active_page = nullptr;
    Page* last_page = nullptr;
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
    
    // tmp will eventually be handled by controller internally
    bool waiting_for_js_button_up = false;
    
    void press_button();
};

} // namepsace ss


