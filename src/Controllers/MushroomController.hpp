#pragma once

#include "States/EntityState.hpp"

#include "Controllers/PlayerController.hpp"
#include "Controllers/CollisionController.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/PointsParticlesController.hpp"

#include "Window.hpp"

static auto mushroom_controller(MushroomState& mushroom, LevelState& level){
  entity_gravity(mushroom, level);
  entity_movement(mushroom, level);
  entity_turn_around(mushroom, config::MushroomWalkSpeed);

  for (auto& p : mushroom.points_manager.points){
    points_particles_controller(p);
  }

  if (mushroom.is_active) return;

  if (mushroom.should_be_pushed_out && mushroom.offset < 1.f){
    const auto value = window::delta_time * 2;

    mushroom.offset += value;
    mushroom.position.y -= value * 60.f;
  }

  if (mushroom.offset >= 1.f){
    mushroom.is_active = true;
  }
}
