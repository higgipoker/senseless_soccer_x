#include "PageCalibrate.hpp"
#include "Menu.hpp"
#include "menu_tools.hpp"
#include "WidgetLabel.hpp"
#include "ListBox.hpp"
#include "../global.hpp"
#include "../math/vector.hpp"
#include "../sfml/sfml_tools.hpp"

namespace ss {

PageCalibrate::PageCalibrate (sf::RenderWindow& wnd, Menu* ctx) : Page (wnd, ctx, Page_ID::Calibrate) {
    Widget* button1 = addChild (make_button (window, MenuEvent::Exit,           "BACK",    {140, 600}, color_ext));
    Widget* button2 = addChild (make_button (window, MenuEvent::CalibrateDone,  "DONE",    {660, 600}, color_std));

    active_widget = button2;



    button1->onDisable();

    std::unique_ptr label = std::make_unique<Label> (window, global::Resources::font_std, 18, sf::Color::White);
    label->setPosition ({650, 260});
    label->setText ("MOVE THE LEFT ANALOG STICK TO ALL EXTREMITIES");
    addChild (std::move (label));

    std::unique_ptr listbox = std::make_unique<ListBox> (window, sf::Vector2f (140, 250), sf::Vector2f (500, 300));

   // Widget* container = addChild (std::move (listbox));
    std::array<Widget*, NUMBER_ROWS> rows;
    size_t i = 0;
    for (auto& row : listbox->data) {
        rows[i] = addChild (std::move (row));
        ++i;
    }

    for (size_t i = 0; i < rows.size(); ++i) {
        if (i > 0) {
            rows[i]->neighbours.above = rows[i - 1];
        }
        if (i < rows.size() - 1) {
            rows[i]->neighbours.below = rows[i + 1];
        }
    }

    rows[NUMBER_ROWS-1]->neighbours.below = button2;
    button2->neighbours.above = rows[NUMBER_ROWS-1];

    button1->neighbours.right = button2;
    button2->neighbours.left = button1;
}

void PageCalibrate::onShow() {
    global::log ("reading gamepads...");
    phase = TestPhase::Extremities;
    context->gamepad_enabled = false;
    //
    // gamepad widget
    //
    std::unique_ptr<Widget> w = std::make_unique<GamepadWidget> (window);
    gamepad_widget = static_cast<GamepadWidget*> (addChild (std::move (w)));
    gamepad_widget->setSize ({250, 200});
    gamepad_widget->setPosition ({780, 350});
    //gamepad_widget->controller.unCalibrate();
    //calibration.reset();


    //
    // read connected joysticks
    //
    joysticks  = sfml::connected_joysticks();
    gamepad_widget->controller.unCalibrate();
    gamepad_widget->gamepad.suspended = true;
    read_dead_zone = true;
    for (size_t i = 0; i < joysticks.size(); ++i) {
        if (joysticks[i]) {
            global::log ("joystick connected with id :" + std::to_string (i));
        }
    }
}

void PageCalibrate::onHide() {
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
            global::log ("done calibrating extremities");
            global::log (vec_print (calibration.extremities.min));
            global::log (vec_print (calibration.extremities.max));
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
        global::log ("CALIBRATE DONE");
        phase_done = true;
        break;
    default:
        break;

    }
}
}

