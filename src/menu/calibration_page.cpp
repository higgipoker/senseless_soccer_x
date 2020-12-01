#include "calibration_page.h"
#include "widgets.h"
#include "menu.h"
#include <iostream>
namespace ss {
namespace menu {
namespace calibrate {
void handle_event (const std::string& event, Menu* menu) {
    std::cout << "handle " << event << std::endl;

    if (event.find ("gamepad_list") != std::string::npos) {
        int controller_id = atoi (event.substr (event.find ("."), 2).c_str());
        menu->calibrate_layout.selected_gamepad_index = controller_id;
        std::cout << "selected controller " << controller_id << std::endl;

    } else  if (event == "TEST") {
        if (menu->calibrate_layout.selected_gamepad_index >= 0) {
            attach_controller (&menu->calibrate_layout.gamepad_widget->gamepad, &menu->controllers[menu->calibrate_layout.selected_gamepad_index].state);
        }
        for (int i = 0; i < 8; ++i) {
            set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.listrow[i]], false);
        }

        set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_test], false);
        set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_calibrate], false);
        set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_exit], false);

    } else if(event=="START_CALIBRATE"){
        if (menu->calibrate_layout.selected_gamepad_index >= 0) {
            set_widget_visible(menu->calibrate_layout.calibrate_widget, true);
            attach_controller (&menu->calibrate_layout.calibrate_widget->gamepad, &menu->controllers[menu->calibrate_layout.selected_gamepad_index].state);
        }
        
    } else  if (event == "DONE") {
        // enable widgets
        for (int i = 0; i < 8; ++i) {
            if (menu->calibrate_layout.active_rows[i]) {
                set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.listrow[i]], true);
            }
        }

        set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_test], true);
        set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_calibrate], true);
        set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_exit], true);
    }
}
}

} // namespace
} // namespace
