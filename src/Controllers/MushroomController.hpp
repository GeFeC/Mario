#pragma once

#include "States/EntityState.hpp"

#include "Controllers/PlayerController.hpp"
#include "Controllers/CollisionController.hpp"

#include "Window.hpp"

static auto mushroom_controller(MushroomState& mushroom, EntityState& player){
  if (mushroom.is_active) return;

  if (player_hit_block_above(player, BouncingBlockState(mushroom.position / 60.f))){
    mushroom.should_be_pushed_out = true;
    mushroom.is_visible = true;
  } 

  if (mushroom.should_be_pushed_out && mushroom.offset < 1.f){
    const auto value = window::delta_time * 2;

    mushroom.offset += value;
    mushroom.position.y -= value * 60.f;
  }

  if (mushroom.offset >= 1.f){
    mushroom.is_active = true;
  }
}
