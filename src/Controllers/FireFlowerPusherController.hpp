#pragma once

#include "Controllers/FireFlowerController.hpp"
#include "States/FireFlowerPusherState.hpp"
#include "States/LevelState.hpp"

namespace mario::pusher_controller{

static auto push_out(FireFlowerPusherState& pusher, LevelState& level){
  auto& flower = pusher.fire_flower;

  flower.should_be_pushed_out = true;
  flower.is_visible = true;
}

static auto controller(FireFlowerPusherState& pusher, LevelState& level){
  auto& flower = pusher.fire_flower;

  fire_flower_controller(flower, level);
  flower.texture = &textures::fire_flower[level.fire_flower_blink_counter.int_value()];

  if (flower.should_be_pushed_out && flower.offset < 1.f){
    const auto value = window::delta_time * 2;

    flower.offset += value;
    flower.position.y -= value * BlockBase::Size;
  }
}

} //namespace mario::pusher_controller
