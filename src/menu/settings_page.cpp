#include "settings_page.h"
#include "widgets.h"
#include "menu.h"
#include "../files/files.h"
#include "../math/vector.hpp"
#include <iostream>
namespace ss {
namespace menu {
namespace settings {

void show_calibrate_phase1 (Menu* menu) {
    menu->state                     = Menu_State::State_CalibratePhase1;
    menu->gamepad_state.suspended   = true;

    if (menu->settings_layout.selected_gamepad_index >= 0) {
        Calibration* cali               = &menu->calibrations[menu->active_controller];
        disable_left_widgets (menu);
        hide_gamepad_widget (menu);
        set_widget_visible (menu->settings_layout.leftstick_diag, true);
        set_widget_visible (menu->settings_layout.rightstick_diag, true);
        set_widget_visible (menu->settings_layout.cali_instructions, true);
        set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_cancel], true);
        set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_done], true);
        label (menu, menu->settings_layout.cali_instructions->label.text).setString ("STEP 1 OF 2: \nLEAVE THE STICKS CENTERED AND PRESS A BUTTON");
        
        // attach controller to leftstick widget
        attach_controller (&menu->settings_layout.leftstick_diag->thumbstick_diagnostic, Thumbstick_Diagnostic_Widget::Left, &menu->controllers[menu->settings_layout.selected_gamepad_index].state, cali);
        
        // attach controller to right stick widget
        attach_controller (&menu->settings_layout.rightstick_diag->thumbstick_diagnostic, Thumbstick_Diagnostic_Widget::Right, &menu->controllers[menu->settings_layout.selected_gamepad_index].state, cali);
        
        set_active_widget (&menu->page_settings[menu->settings_layout.widget_idx.btn_done], menu);
        on_calibration_started (cali);
    }
}

void show_calibrate_phase2 (Menu* menu) {
    Calibration* cali                   = &menu->calibrations[menu->active_controller];
    cali->left_stick.at_rest_before     = menu->controllers[menu->settings_layout.selected_gamepad_index].state.left_stick_raw;
    cali->right_stick.at_rest_before    = menu->controllers[menu->settings_layout.selected_gamepad_index].state.right_stick_raw;
    menu->state                         = Menu_State::State_CalibratePhase2;
    label (menu, menu->settings_layout.cali_instructions->label.text).setString ("STEP 2 OF 2: \nMOVE BOTH THUMBSTICKS TO THE EXTREMITIES AND \nPRESS A BUTTON");
}

void calibrate_finished (Menu* menu) {
    menu->state = Menu_State::State_SettingsPage;
    menu->gamepad_state.suspended = false;
    detatch_controller (&menu->settings_layout.rightstick_diag->thumbstick_diagnostic);
    detatch_controller (&menu->settings_layout.leftstick_diag->thumbstick_diagnostic);
    Calibration* cali = &menu->calibrations[menu->active_controller];
    cali->left_stick.at_rest_after = menu->controllers[menu->settings_layout.selected_gamepad_index].state.left_stick_raw;
    cali->right_stick.at_rest_after = menu->controllers[menu->settings_layout.selected_gamepad_index].state.right_stick_raw;
    on_calibration_finished (cali, menu->settings_layout.selected_gamepad_index);
    get_selected_controller (menu)->calibrate (cali);
    enable_left_widgets (menu);
    set_widget_visible (menu->settings_layout.leftstick_diag, false);
    set_widget_visible (menu->settings_layout.rightstick_diag, false);
    set_widget_visible (menu->settings_layout.cali_instructions, false);
    set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_cancel], false);
    set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_done], false);
    show_gamepad_widget (menu);
    set_active_widget (&menu->page_settings[menu->settings_layout.widget_idx.btn_calibrate], menu);
}

void cancel_calibrate (Menu* menu) {
    calibrate_finished (menu);
    menu->gamepad_state.suspended = false;
}

void show_test (Menu* menu) {
    menu->state = Menu_State::State_Test;
    menu->gamepad_state.suspended = true;
    // attach to left stick widget
    attach_controller (&menu->page_settings[menu->settings_layout.gamepad_widget->gamepad.left_stick].thumbstick_test, Thumbstick_Test_Widget::Left, &menu->controllers[menu->settings_layout.selected_gamepad_index].state);

    // attach to right stick widget
    attach_controller (&menu->page_settings[menu->settings_layout.gamepad_widget->gamepad.right_stick].thumbstick_test, Thumbstick_Test_Widget::Right, &menu->controllers[menu->settings_layout.selected_gamepad_index].state);

    set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_done], true);
    set_active_widget (&menu->page_settings[menu->settings_layout.widget_idx.btn_done], menu);
    disable_left_widgets (menu);

    // calibrate widget
    hide_calibrate_widget (menu);
}

void test_finished (Menu* menu) {
    menu->state = Menu_State::State_SettingsPage;
    menu->gamepad_state.suspended = false;
    detatch_controller (&menu->page_settings[menu->settings_layout.gamepad_widget->gamepad.left_stick].thumbstick_test);
    detatch_controller (&menu->page_settings[menu->settings_layout.gamepad_widget->gamepad.right_stick].thumbstick_test);
    enable_left_widgets (menu);
    // done and cancel
    set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_cancel], false);
    set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_done], false);
    set_active_widget (&menu->page_settings[menu->settings_layout.widget_idx.btn_test], menu);
}

void disable_left_widgets (Menu* menu) {
    set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_test], false);
    set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_calibrate], false);
    set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_exit], false);
    for (int i = 0; i < 8; ++i) {
        if (menu->settings_layout.active_rows[i]) {
            set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.listrow[i]], false);
        }
    }

}

void enable_left_widgets (Menu* menu) {
    set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_test], true);
    set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_calibrate], true);
    set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.btn_exit], true);
    for (int i = 0; i < 8; ++i) {
        if (menu->settings_layout.active_rows[i]) {
            set_widget_enabled (&menu->page_settings[menu->settings_layout.widget_idx.listrow[i]], true);
        }
    }
    set_widget_visible (menu->settings_layout.gamepad_widget, true);
    set_widget_visible (&menu->page_settings[menu->settings_layout.gamepad_widget->gamepad.left_stick], true);
    set_widget_visible (&menu->page_settings[menu->settings_layout.gamepad_widget->gamepad.right_stick], true);

}

void show_gamepad_widget (Menu* menu) {
    set_widget_visible (menu->settings_layout.gamepad_widget, true);
    set_widget_visible (&menu->page_settings[menu->settings_layout.gamepad_widget->gamepad.left_stick], true);
    set_widget_visible (&menu->page_settings[menu->settings_layout.gamepad_widget->gamepad.right_stick], true);
}

void hide_gamepad_widget (Menu* menu) {
    set_widget_visible (menu->settings_layout.gamepad_widget, false);
    set_widget_visible (&menu->page_settings[menu->settings_layout.gamepad_widget->gamepad.left_stick], false);
    set_widget_visible (&menu->page_settings[menu->settings_layout.gamepad_widget->gamepad.right_stick], false);
}

void show_calibrate_widget (Menu* menu) {
    set_widget_visible (menu->settings_layout.leftstick_diag, true);
    set_widget_visible (menu->settings_layout.rightstick_diag, true);
}

void hide_calibrate_widget (Menu* menu) {
    set_widget_visible (menu->settings_layout.leftstick_diag, false);
    set_widget_visible (menu->settings_layout.rightstick_diag, false);
}
} // namespace
} // namespace
} // namespace
