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

  entity_gravity(mushroom, level);
  entity_movement(mushroom, level);
  entity_turn_around(mushroom);

  for (auto& p : mushroom.points_generator.items){
    points_particles_controller(p);
  }
}

static auto green_mushroom_controller(MushroomState& mushroom, LevelState& level){
  auto& player = level.player;
  if (player.is_dead) return;

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
  if (player.is_dead) return;

  if (collision::is_hovering(player, mushroom) && mushroom.is_active){
    auto& points = mushroom.points_generator.item();
    points.set_active(mushroom.reward_for_killing, mushroom.position);
  
    mushroom.disappear();

    if (player.growth == PlayerState::Growth::Small){
      player.is_growing_up = true;
    }
    level.stats.score += mushroom.reward_for_killing;
  }

  mushroom_controller(mushroom, level);
}
