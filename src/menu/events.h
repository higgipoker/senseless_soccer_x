#pragma once

namespace ss {
namespace menu {

enum class Event : int {
    // universal
    None,
    Back,
    Done,
    Cancel,
    Exit,
    Up,
    Down,
    Left,
    Right,
    Fire,

    // page main
    PageMain,

    // page calibrate
    PageSettings,
    Test,
    Calibrate,

    // special widget types
    ListRow1,
    ListRow2,
    ListRow3,
    ListRow4,
    ListRow5,
    ListRow6,
    ListRow7,
    ListRow8,
};

}// namespace
}// namespace
