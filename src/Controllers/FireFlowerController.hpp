#pragma once

#include "States/EntityState.hpp"
#include "States/BlockState.hpp"

#include "Controllers/PlayerController.hpp"

#include "Window.hpp"

static auto fire_flower_controller(FireFlowerState& flower, PlayerState& player){
  if (player_hit_block_above(player, BouncingBlockState(flower.position / 60.f))){
    flower.should_be_pushed_out = true;
    flower.is_visible = true;
  } 

  if (flower.should_be_pushed_out && flower.offset < 1.f){
    const auto value = window::delta_time * 2;

    flower.offset += value;
    flower.position.y -= value * 60.f;
  }
}
