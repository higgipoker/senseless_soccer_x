#pragma once
#include "events.h"
#include <string>
namespace ss {
namespace menu {
struct Widget;
struct Menu;
namespace settings {

struct Settings_Page_Layout {
    int first_row_index                 {0};
    int number_rows                     {8};
    int selected_gamepad_index          {-1};
    bool at_least_one_gamepad_found     {false};
    Widget* gamepad_widget              {nullptr};
    Widget* leftstick_diag              {nullptr};
    Widget* rightstick_diag             {nullptr};
    Widget* cali_instructions           {nullptr};

    struct {
        int listbox_title = -1;
        int listbox_frame = -1;
        int listrow[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
        int btn_test = -1;
        int btn_calibrate = -1;
        int btn_exit = -1;
        int btn_done = -1;
        int btn_cancel = -1;

    } widget_idx;
    int active_rows[8] = {0, 0, 0, 0, 0, 0, 0, 0};
};

void handle_event (const Event event, Menu* menu);

void show_phase_1(Menu* menu);
void show_phase_2(Menu* menu);
void calibration_finished(Menu* menu);
void cancel(Menu* menu);
}
}
}
