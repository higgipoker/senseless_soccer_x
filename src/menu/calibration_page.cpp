#include "calibration_page.h"
#include "widgets.h"
#include "menu.h"
#include <iostream>
namespace ss {
namespace menu {
namespace calibrate {
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
        menu->calibrate_layout.selected_gamepad_index = controller_id;
        std::cout << "selected controller " << std::to_string(controller_id) << std::endl;
        break;

    case Event::Test:
        if (menu->calibrate_layout.selected_gamepad_index >= 0) {
            attach_controller (&menu->calibrate_layout.gamepad_widget->gamepad, &menu->controllers[menu->calibrate_layout.selected_gamepad_index].state);
        }
        for (int i = 0; i < 8; ++i) {
            set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.listrow[i]], false);
        }

        set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_test], false);
        set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_calibrate], false);
        set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_exit], false);
        break;

    case Event::Calibrate:
        if (menu->calibrate_layout.selected_gamepad_index >= 0) {
            set_widget_visible(menu->calibrate_layout.calibrate_widget, true);
            attach_controller (&menu->calibrate_layout.calibrate_widget->gamepad, &menu->controllers[menu->calibrate_layout.selected_gamepad_index].state);
        }
        break;

    case Event::Done:
        for (int i = 0; i < 8; ++i) {
            if (menu->calibrate_layout.active_rows[i]) {
                set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.listrow[i]], true);
            }
        }

        set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_test], true);
        set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_calibrate], true);
        set_widget_enabled (&menu->page_calibrate[menu->calibrate_layout.widget_idx.btn_exit], true);
        break;
    default:
        break;
    }
}
}

} // namespace
} // namespace
