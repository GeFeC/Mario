#pragma once

#include "States/EntityState.hpp"

#include "Controllers/PlayerController.hpp"
#include "Controllers/CollisionController.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/PointsParticlesController.hpp"

#include "Window.hpp"

static auto mushroom_controller(MushroomState& mushroom, LevelState& level){
  //Interaction with blocks
  entity_bounce_when_on_bouncing_block(mushroom, level);

  //Interaction with player
  auto& player = level.player;
  const auto mushroom_block = BouncingBlockState(mushroom.position / config::BlockSize);
  if (player_hit_block_above(player, mushroom_block) && !mushroom.should_be_pushed_out){
    mushroom.should_be_pushed_out = true;
    mushroom.is_visible = true;

    if (player.position.x < mushroom_block.position.x) mushroom.set_direction(EntityState::DirectionRight);
    if (player.position.x > mushroom_block.position.x) mushroom.set_direction(EntityState::DirectionLeft);
  } 

  entity_gravity(mushroom, level);
  entity_movement(mushroom, level);
  entity_turn_around(mushroom);

  for (auto& p : mushroom.points_generator.items){
    points_particles_controller(p);
  }

  if (mushroom.is_active) return;

  if (mushroom.should_be_pushed_out && mushroom.offset < 1.f){
    const auto value = window::delta_time * 2;

    mushroom.offset += value;
    mushroom.position.y -= value * config::BlockSize;
  }

  if (mushroom.offset >= 1.f){
    mushroom.is_active = true;
  }
}

static auto green_mushroom_controller(MushroomState& mushroom, LevelState& level){
  auto& player = level.player;
  if (collision::is_hovering(player, mushroom) && mushroom.is_active){
    auto& points = mushroom.points_generator.item();
    points.set_active("+1 HP", mushroom.position);

    mushroom.disappear();

    level.stats.hp++;
  }

  mushroom_controller(mushroom, level);
}

static auto red_mushroom_controller(MushroomState& mushroom, LevelState& level){
  auto& player = level.player;
  if (collision::is_hovering(player, mushroom) && mushroom.is_active){
    auto& points = mushroom.points_generator.item();
    points.set_active(mushroom.reward_for_killing, mushroom.position);
  
    mushroom.disappear();

    player.is_growing_up = true;
    level.stats.score += mushroom.reward_for_killing;
  }

  mushroom_controller(mushroom, level);
}
