#pragma once

#include "States/MushroomPusherState.hpp"
#include "States/LevelState.hpp"

#include "Controllers/MushroomController.hpp"

static auto pusher_push_out(MushroomPusherState& pusher, LevelState& level){
  auto& mushroom = pusher.mushroom;

  mushroom.should_be_pushed_out = true;
  mushroom.is_visible = true;

  auto& player = level.player;
  const auto mushroom_block = BouncingBlockState(mushroom.position / BlockBase::Size);

  if (player.position.x < mushroom_block.position.x) mushroom.set_direction(EntityState::DirectionRight);
  if (player.position.x > mushroom_block.position.x) mushroom.set_direction(EntityState::DirectionLeft);
}

static auto pusher_controller(MushroomPusherState& pusher, LevelState& level){
  auto& mushroom = pusher.mushroom;

  using MushroomType = MushroomPusherState::MushroomType;
  switch(pusher.mushroom_type){
    case MushroomType::Red: red_mushroom_controller(mushroom, level); break;
    case MushroomType::Green: green_mushroom_controller(mushroom, level); break;
  }

  if (mushroom.is_active) return;

  if (mushroom.should_be_pushed_out && mushroom.offset < 1.f){
    const auto value = window::delta_time * 2;

    mushroom.offset += value;
    mushroom.position.y -= value * BlockBase::Size;
  }

  if (mushroom.offset >= 1.f){
    mushroom.is_active = true;
  }
}
