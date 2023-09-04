#pragma once

namespace settings
{
inline constexpr int initial_screen_width = 1024;
inline constexpr int initial_screen_height = 720;

inline bool is_fullscreen = false;
inline float scale_factor_x = 1.F;
inline float scale_factor_y = 1.F;
// in 1 second object moves 1/5 of the screen on horizontal axis
inline float speed_unit = initial_screen_height / 5.F;
inline float speed_scale_x = 1.F;
inline float speed_scale_y = 1.F;
} // namespace settings
