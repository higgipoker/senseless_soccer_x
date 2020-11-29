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

            // disable widgets
            disable_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.listbox_title]);
            disable_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.listbox_frame]);

            for (int i = 0; i < 8; ++i) {
                disable_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.listrow[i]]);
            }

            disable_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_test]);
            disable_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_calibrate]);
            disable_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_exit]);
        }
    } else  if (event == "DONE") {
        if (menu->calibrate_layout.selected_gamepad_index >= 0) {
            // enable widgets
            enable_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.listbox_title]);
            enable_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.listbox_frame]);

            for (int i = 0; i < 8; ++i) {
                enable_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.listrow[i]]);
            }

            enable_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_test]);
            enable_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_calibrate]);
            enable_widget (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_exit]);
        }
    }
}

}

}
}
