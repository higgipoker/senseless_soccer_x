#include "PageMain.hpp"
#include "Menu.hpp"
#include "menu_tools.hpp"

namespace ss {

const int EXIT = 0;
const int FRIENDLY = 1;
const int LEAGUE = 2;
const int CUP = 3;
const int CAREER = 4;
const int CALIBRATE = 5;

PageMain::PageMain (sf::RenderWindow& wnd, Menu* ctx) : Page (wnd, ctx, Page_ID::Main) {
    float btn_x = window.getSize().x / 2 - 500 / 2;
    Widget* button1 = addChild (make_button (window, FRIENDLY,  "FRIENDLY ",    {btn_x, 250}, color_std));
    Widget* button2 = addChild (make_button (window, LEAGUE,    "LEAGUE",       {btn_x, 320}, color_std));
    Widget* button3 = addChild (make_button (window, CUP,       "CUP",          {btn_x, 390}, color_std));
    Widget* button4 = addChild (make_button (window, CAREER,    "CAREER",       {btn_x, 460}, color_std));
    Widget* button5 = addChild (make_button (window, CALIBRATE, "CALIBRATE",    {btn_x, 530}, color_std));
    Widget* button6 = addChild (make_button (window, EXIT,      "EXIT",         {btn_x, 600}, color_ext));

    button1->neighbours.below = button2;
    button2->neighbours.above = button1;

    button2->neighbours.below = button3;
    button3->neighbours.above = button2;

    button3->neighbours.below = button4;
    button4->neighbours.above = button3;

    button4->neighbours.below = button5;
    button5->neighbours.above = button4;

    button5->neighbours.below = button6;
    button6->neighbours.above = button5;

    active_widget = button1;
}

void PageMain::handleButtonPress(Widget* button){
    if(button->id == CALIBRATE){
        context->changePage(Page_ID::Calibrate);
    }
}
}
