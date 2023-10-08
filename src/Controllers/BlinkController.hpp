#pragma once

#include "States/BlockState.hpp"
#include "Window.hpp"
#include <vector>

namespace mario::blink_controller{

static auto run(){
  static auto blink_counter = 0.f;
  auto blink_state = 0;

  if (blink_counter < 3.f && blink_counter > 0.f){
    blink_state = blink_counter;
  }
  else{
    blink_state = 0;
  }

  blink_counter += window::delta_time * 10.f;

  if (blink_counter >= 10.f){
    blink_counter = 0.f;
  }

  return blink_state;
}

} //namespace mario::blink_controller
