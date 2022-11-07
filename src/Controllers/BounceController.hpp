#pragma once

#include "Controllers/CollisionController.hpp"

#include "States/EntityState.hpp"
#include "States/BlockState.hpp"

#include "Window.hpp"
#include "Util.hpp"

#include <iostream>

namespace bounce{
  inline auto start(BlockState& block){
    auto& bounce_state = block.bounce_state;

    bounce_state.is_bouncing = true;
    bounce_state.temp_y = block.position.y;
    bounce_state.power = bounce_state.initial_power;
  }
}
inline auto bounce_controller(BlockState& block, EntityState& player){
  auto& bounce_state = block.bounce_state;

  if (bounce_state.is_bouncing){
    block.position.y += bounce_state.power;
    bounce_state.power += window::delta_time * 60.f;

    if (block.position.y >= bounce_state.temp_y){
      bounce_state.is_bouncing = false;
      bounce_state.power = 0.f;

      block.position.y = bounce_state.temp_y;
    }
  }
}
