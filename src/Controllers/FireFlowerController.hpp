#pragma once

#include "States/EntityState.hpp"
#include "States/BlockState.hpp"

#include "Controllers/PlayerController.hpp"
#include "Controllers/PointsParticlesController.hpp"

#include "Window.hpp"

static auto fire_flower_controller(FireFlowerState& flower, PlayerState& player, StatsState& stats){
  flower.texture = &textures::fire_flower[LevelState::fire_flower_blink_counter.int_value()];

  for (auto& p : flower.points_generator.items){
    points_particles_controller(p);
  }

  if (player_hit_block_above(player, BouncingBlockState(flower.position / 60.f))){
    flower.should_be_pushed_out = true;
    flower.is_visible = true;
  } 

  if (flower.should_be_pushed_out && flower.offset < 1.f){
    const auto value = window::delta_time * 2;

    flower.offset += value;
    flower.position.y -= value * 60.f;
  }

  //Interaction with player
  const auto is_player_big = player.growth == PlayerState::Growth::Big;
  if (collision::is_hovering(player, flower) && flower.is_visible && is_player_big){
    flower.points_generator.item().set_active(
      FireFlowerState::RewardForEating,
      flower.position
    );
    stats.score += FireFlowerState::RewardForEating;
  
    flower.is_visible = false;
    flower.position.y = config::BigValue;
    player.is_changing_to_fire = true;
  }
}
