#pragma once

#include "Window.hpp"

inline auto looped_counter(float counter, float speed, float limit){
  counter += window::delta_time * speed;
  if (counter >= limit) counter = 0.f;

  return counter;
}
