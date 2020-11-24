// #pragma once
// #include <SFML/Window/Event.hpp>
// #include <map>
// #include <set>
// #include <vector>
// 
// namespace ss {
// 
// const unsigned char mask_zero{0};        // 0000 0000
// const unsigned char mask_a{1 << 0};      // 0000 0001
// const unsigned char mask_b{1 << 1};      // 0000 0010
// const unsigned char mask_x{1 << 2};      // 0000 0100
// const unsigned char mask_y{1 << 3};      // 0000 1000
// const unsigned char mask_l1{1 << 4};     // 0001 0000
// const unsigned char mask_r1{1 << 5};     // 0010 0000
// const unsigned char mask_back{1 << 6};   // 0100 0000
// const unsigned char mask_start{1 << 7};  // 1000 0000
// 
// const unsigned char mask_dpad_up{1 << 0};      // 0000 0001
// const unsigned char mask_dpad_right{1 << 1};   // 0000 0010
// const unsigned char mask_dpad_down{1 << 2};    // 0000 0100
// const unsigned char mask_dpad_left{1 << 3};    // 0000 1000
// const unsigned char mask_stick_up{1 << 4};     // 0001 0000
// const unsigned char mask_stick_right{1 << 5};  // 0010 0000
// const unsigned char mask_stick_down{1 << 6};   // 0100 0000
// const unsigned char mask_stick_left{1 << 7};   // 1000 0000
// 
// enum class Buttons {
//     DPadUp = 0,
//     DPadDown,
//     DPadLeft,
//     DPadRight,
// 
//     Button1,
//     Button2,
//     Button3,
//     Button4,
// 
//     ShoulderLeft1,
//     ShoulderLeft2,
//     Shoulderright1,
//     Shoulderright2,
// 
//     Start,
//     Select
// };
// 
// enum class InputEvent { FireDown = 0, FireUp, SingleTap, DoubleTap };
// static std::map<InputEvent, std::string> event_to_string{
//     {InputEvent::FireDown,  "FireDown"},
//     {InputEvent::FireUp,    "FireUp"},
//     {InputEvent::SingleTap, "SingleTap"},
//     {InputEvent::DoubleTap, "DoubleTap"},
// };
// 
// class Controllable {
// public:
//     virtual ~Controllable () = default;
//     virtual void onInputEvent (const InputEvent in_event, const std::vector<int>& in_params) = 0;
//     void attachInput();
// 
//     static std::string toString (const InputEvent in_event) {
//         return event_to_string.at (in_event);
//     }
// };
// 
// class InputDevice {
// public:
//     InputDevice () = default;
//     virtual ~InputDevice () = default;
// 
//     virtual void update () = 0;
//     virtual sf::Vector2f getAxisPosition() = 0;
//     bool up ();
//     bool down ();
//     bool left ();
//     bool right ();
//     bool fireDown ();
//     bool isButtonPressed (const unsigned char in_which);
//     void attachListener (Controllable& in_listener);
//     void detatchListener (Controllable& in_listener);
// 
// protected:
//     struct {
//         int FireLength          = 0;
//         int FireLengthCached    = 0;
//         int fire_ticks          = 0;
//         int ticks_since_tap     = 0;
//         bool cached_tap         = false;
// 
//         int fire_tap_length = 4;
//         int fire_double_tap_length = 12;
//     } fire_params;
// 
//     unsigned char buttonmask{0x0};
//     unsigned char directionmask{0x0};
//     unsigned char old_buttonmask{0x0};
// 
//     void notify (const InputEvent in_event, const std::vector<int>& in_params);
// 
//     std::set<Controllable*> listeners;
// };
// 
// }  // namespace Senseless
