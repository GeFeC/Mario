#pragma once

#include "Window.hpp"

inline auto animation_controller(){
  static auto animation_state = 0.f;

  animation_state += window::delta_time * 20.f;
  if (animation_state >= 4.f) animation_state = 0.f;

  return static_cast<int>(animation_state);
}
