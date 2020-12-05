#include "settings_page.h"
#include "widgets.h"
#include "menu.h"
#include "../files/files.h"
#include <iostream>
namespace ss {
namespace menu {
namespace settings {

void show_phase_1 (Menu* menu) {
    menu->state = Menu_State::State_CalibratePhase1;
    if (menu->settings_layout.selected_gamepad_index >= 0) {
        //
        // stuff on the left side of the screen
        //
        set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_test], false);
        set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_calibrate], false);
        set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_exit], false);
        for (int i = 0; i < 8; ++i) {
            if (menu->settings_layout.active_rows[i]) {
                set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.listrow[i]], false);
            }
        }
        //

        //
        // stuff on the right side of the screen
        //
        label (menu, menu->settings_layout.cali_instructions->label.text).setString ("LEAVE THE STICKS CENTERED AND PRESS A BUTTON");
        set_widget_visible (menu->settings_layout.gamepad_widget, false);
        set_widget_visible (menu->settings_layout.leftstick_diag, true);
        set_widget_visible (menu->settings_layout.rightstick_diag, true);
        set_widget_visible (menu->settings_layout.cali_instructions, true);
        // done and cancel
        set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_cancel], true);
        set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_done], true);
        menu->active_widget = &menu->page_settings[menu->settings_layout.widget_idx.btn_done];

        // attach the left stick to left cali widget
        attach_controller (&menu->settings_layout.leftstick_diag->thumbstick_diagnostic, Thumbstick_Diagnostic_Widget::Left, &menu->controllers[menu->settings_layout.selected_gamepad_index].state, &menu->controllers[menu->settings_layout.selected_gamepad_index].act_calibration);

        // attach the right stick to right cali widget
        attach_controller (&menu->settings_layout.rightstick_diag->thumbstick_diagnostic, Thumbstick_Diagnostic_Widget::Right, &menu->controllers[menu->settings_layout.selected_gamepad_index].state, &menu->controllers[menu->settings_layout.selected_gamepad_index].act_calibration);
    }
}

void show_phase_2 (Menu* menu) {
    menu->state = Menu_State::State_CalibratePhase2;
    label (menu, menu->settings_layout.cali_instructions->label.text).setString ("MOVE BOTH THUMBSTICKS TO THE EXTREMITIES \nAND PRESS A BUTTON");
}

void calibration_finished (Menu* menu) {
    menu->state = Menu_State::State_SettingsPage;
    //
    //  stuff on the left side of the screen
    //
    set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_test], true);
    set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_calibrate], true);
    set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_exit], true);
    for (int i = 0; i < 8; ++i) {
        if (menu->settings_layout.active_rows[i]) {
            set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.listrow[i]], true);
        }
    }
    menu->active_widget = &menu->page_settings[menu->settings_layout.widget_idx.btn_calibrate];
    //

    //
    // stuff on the right side of the screen
    //
    set_widget_visible (menu->settings_layout.gamepad_widget, true);
    set_widget_visible (menu->settings_layout.leftstick_diag, false);
    set_widget_visible (menu->settings_layout.rightstick_diag, false);
    set_widget_visible (menu->settings_layout.cali_instructions, false);
    // done and cancel
    set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_cancel], false);
    set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_done], false);
    //
    // save the calibration to file
    //
    files::File file;
    files::open(file, files::working_dir() +"/data/calibration.cfg");
    
    files::close(file);
    
    
}

void cancel(Menu* menu){
    calibration_finished(menu);
}

void handle_event (const Event event, Menu* menu) {
    int controller_id = 0;

    switch (event) {

    case Event::ListRow1:
        controller_id = 0;
    case Event::ListRow2:
        controller_id = 1;
    case Event::ListRow3:
        controller_id = 2;
    case Event::ListRow4:
        controller_id = 3;
    case Event::ListRow5:
        controller_id = 4;
    case Event::ListRow6:
        controller_id = 5;
    case Event::ListRow7:
        controller_id = 6;
    case Event::ListRow8:
        controller_id = 7;
        menu->settings_layout.selected_gamepad_index = controller_id;
        std::cout << "selected controller " << std::to_string (controller_id) << std::endl;
        break;

    case Event::Test:
        if (menu->settings_layout.selected_gamepad_index >= 0) {
            attach_controller (&menu->settings_layout.gamepad_widget->gamepad, &menu->controllers[menu->settings_layout.selected_gamepad_index].state);
        }
        for (int i = 0; i < 8; ++i) {
            set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.listrow[i]], false);
        }

        set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_test], false);
        set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_calibrate], false);
        set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_exit], false);
        break;

    case Event::Calibrate:
        menu->state = Menu_State::State_CalibratePhase1;
        if (menu->settings_layout.selected_gamepad_index >= 0) {
            set_widget_visible (menu->settings_layout.gamepad_widget, false);
            set_widget_visible (menu->settings_layout.leftstick_diag, true);
            set_widget_visible (menu->settings_layout.rightstick_diag, true);
            set_widget_visible (menu->settings_layout.cali_instructions, true);
            attach_controller (&menu->settings_layout.leftstick_diag->thumbstick_diagnostic, Thumbstick_Diagnostic_Widget::Left, &menu->controllers[menu->settings_layout.selected_gamepad_index].state, &menu->controllers[menu->settings_layout.selected_gamepad_index].act_calibration);
            attach_controller (&menu->settings_layout.rightstick_diag->thumbstick_diagnostic, Thumbstick_Diagnostic_Widget::Right, &menu->controllers[menu->settings_layout.selected_gamepad_index].state, &menu->controllers[menu->settings_layout.selected_gamepad_index].act_calibration);
        }
        break;

    case Event::Done:
        for (int i = 0; i < 8; ++i) {
            if (menu->settings_layout.active_rows[i]) {
                set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.listrow[i]], true);
            }
        }

        set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_test], true);
        set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_calibrate], true);
        set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_exit], true);
        break;
    default:
        break;
    }
}
}

} // namespace
} // namespace
