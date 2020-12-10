// #include "SpriteAnimation.hpp"
// 
// namespace ss {
// 
// void SpriteAnimation::init (const AnimationDefinition& def) {
//     anim_def = def;
// }
// 
// void SpriteAnimation::start() {
//     running = true;
// }
// 
// void SpriteAnimation::stop(){
//     running = false;
// }
// 
// void SpriteAnimation::update() {
//     if (!running) return;
//     if (++ticks >= anim_def.frames_per_frame) {
//         ticks = 0;
//         if (++current_frame >= anim_def.number_frames) {
//             current_frame = 0;
//             if (!anim_def.loop) {
//                 stop();
//                 return;
//             }
//         }
//         act_frame = anim_def.frames[current_frame];
//     }
// }
// 
// }
