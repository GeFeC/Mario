#pragma once

#include "Window.hpp"
#include "States/StatsState.hpp"

namespace mario::stats_controller{

static auto run(StatsState& stats){
  //Time:
  static auto counter = 0.f;
  counter += window::delta_time * 10.f;

  if (stats.time == 0) return;

  if (counter >= 5.f){
    --stats.time;
    counter = 0.f;
  }

  //View movement:

  stats.position_y += window::delta_time * 2000.f * stats.move_direction.as_int();

  stats.position_y = std::clamp(stats.position_y, StatsState::MinPositionY, StatsState::MaxPositionY);
}

} //namespace mario
