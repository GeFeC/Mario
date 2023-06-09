#pragma once

#include "States/LevelState.hpp"
#include "Window.hpp"

namespace mario{

inline auto stats_controller(StatsState& stats){
  static auto counter = 0.f;
  counter += window::delta_time * 10.f;

  if (stats.time == 0) return;

  if (counter >= 5.f){
    --stats.time;
    counter = 0.f;
  }
}

} //namespace mario