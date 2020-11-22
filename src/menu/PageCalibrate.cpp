#include "PageCalibrate.hpp"
#include "Menu.hpp"
#include "menu_tools.hpp"
#include "../math/vector.hpp"
#include "../sfml/sfml_tools.hpp"

namespace ss {

PageCalibrate::PageCalibrate (sf::RenderWindow& wnd, Menu* ctx) : Page (wnd, ctx, Page_ID::Calibrate) {
    Widget* button1 = addChild (make_button (window, MenuEvent::CalibrateDone,  "DONE",    {500, 250}, color_std));
    Widget* button2 = addChild (make_button (window, MenuEvent::Exit,  "BACK",    {500, 310}, color_ext));
    
    button1->neighbours.below = button2;
    button2->neighbours.above = button1;

}

void PageCalibrate::onShow() {
    std::cout << "reading gamepads..." << std::endl;
    phase = TestPhase::Extremities;
    context->gamepad_enabled = false;
    //
    // gamepad widget
    //
    std::unique_ptr<Widget> w = std::make_unique<GamepadWidget> (window);
    gamepad_widget = static_cast<GamepadWidget*> (addChild (std::move (w)));
    gamepad_widget->setSize ({250, 200});
    gamepad_widget->setPosition ({150, 350});
    gamepad_widget->controller.unCalibrate();
    calibration.reset();

    //
    // read connected joysticks
    //
    joysticks  = sfml::connected_joysticks();
    gamepad_widget->controller.unCalibrate();
    gamepad_widget->gamepad.suspended = true;
    read_dead_zone = true;
    for (size_t i = 0; i < joysticks.size(); ++i) {
        if (joysticks[i]) {
            std::cout << "joystick connected with id :" << i << std::endl;
        }
    }
}

void PageCalibrate::onHide(){
    context->gamepad_enabled = true;
}

void PageCalibrate::update() {

    switch (phase) {
    case TestPhase::Idle:
        break;
    case TestPhase::At_Rest:
        calibration.at_rest.min.x = std::min (calibration.at_rest.min.x, gamepad_widget->controller.state.left_stick_vector.x);
        calibration.at_rest.min.y = std::min (calibration.at_rest.min.y, gamepad_widget->controller.state.left_stick_vector.y);
        calibration.at_rest.max.x = std::max (calibration.at_rest.max.x, gamepad_widget->controller.state.left_stick_vector.x);
        calibration.at_rest.max.y = std::max (calibration.at_rest.max.y, gamepad_widget->controller.state.left_stick_vector.y);

        if (phase_done) {
            phase_done = false;
            phase = TestPhase::Idle;
        }
        break;

    case TestPhase::Extremities:
       std::cout << gamepad_widget->controller.state.left_stick_vector.x << " , " << gamepad_widget->controller.state.left_stick_vector.y << std::endl; 
        
        calibration.extremities.min.x = std::min (calibration.extremities.min.x, gamepad_widget->controller.state.left_stick_vector.x);

        calibration.extremities.min.y = std::min (calibration.extremities.min.y, gamepad_widget->controller.state.left_stick_vector.y);

        calibration.extremities.max.x = std::max (calibration.extremities.max.x, gamepad_widget->controller.state.left_stick_vector.x);

        calibration.extremities.max.y = std::max (calibration.extremities.max.y, gamepad_widget->controller.state.left_stick_vector.y);

        if (phase_done) {
            phase_done = false;
            phase = TestPhase::Idle;
            gamepad_widget->gamepad.suspended = false;

            // tmp sane values for rest calibration
            calibration.at_rest.min = {0, 0};

            gamepad_widget->controller.calibrate (calibration, calibration);
            std::cout << "done calibrating extremities" << std::endl;
            std::cout << vec_print (calibration.extremities.min) << std::endl;
            std::cout << vec_print (calibration.extremities.max) << std::endl;
        }
        break;
    }

    Widget::update();

}

void PageCalibrate::draw_self() {
    Page::draw_self();
}

void PageCalibrate::handleMenuEvent (const MenuEvent evt) {
    switch (evt) {
    case MenuEvent::CalibrateDone:
        std::cout << "CALIBRATE DONE" << std::endl;
        phase_done = true;
        break;
    default:
        break;

    }
}
}

