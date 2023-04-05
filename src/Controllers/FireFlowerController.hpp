#pragma once

#include "States/EntityState.hpp"
#include "States/BlockState.hpp"

#include "Controllers/PlayerController.hpp"
#include "Controllers/PointsParticlesController.hpp"

#include "Window.hpp"

static auto fire_flower_controller(FireFlowerState& flower, LevelState& level){
  flower.texture = &textures::fire_flower[level.fire_flower_blink_counter.int_value()];

  for (auto& p : flower.points_generator.items){
    points_particles_controller(p);
  }

  auto& player = level.player;
  if (player_hit_block_above(player, BouncingBlockState(flower.position / config::BlockSize))){
    flower.should_be_pushed_out = true;
    flower.is_visible = true;
  } 

  if (flower.should_be_pushed_out && flower.offset < 1.f){
    const auto value = window::delta_time * 2;

    flower.offset += value;
    flower.position.y -= value * config::BlockSize;
  }

  //Interaction with player
  if (collision::is_hovering(player, flower) && flower.is_visible){
    flower.points_generator.item().set_active(
      FireFlowerState::RewardForEating,
      flower.position
    );
    level.stats.score += FireFlowerState::RewardForEating;
  
    flower.is_visible = false;
    flower.position.y = config::BigValue;

    const auto is_player_big = player.growth == PlayerState::Growth::Big;
    if (is_player_big){
      player.is_changing_to_fire = true;
    }
    else{
      player.is_growing_up = true;
    }
  }
}
