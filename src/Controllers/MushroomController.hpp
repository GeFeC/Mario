#pragma once

#include "States/EntityState.hpp"

#include "Controllers/PlayerController.hpp"
#include "Controllers/CollisionController.hpp"
#include "Controllers/MonsterController.hpp"
#include "Controllers/PointsParticlesController.hpp"

#include "Window.hpp"

namespace mario::mushroom_controller{

static auto run_controller_base(MushroomState& mushroom, LevelState& level){
  //Interaction with blocks
  monster_controller::bounce_when_on_bouncing_block(mushroom, level);

  entity_controller::handle_gravity(mushroom, level);
  entity_controller::handle_movement(mushroom, level);
  monster_controller::handle_turning_around(mushroom);

  for (auto& p : mushroom.points_generator.items){
    points_particles_controller::run(p);
  }
}

static auto run_green_mushroom_controller(MushroomState& mushroom, LevelState& level){
  auto& player = level.player;
  if (player.is_dead) return;

  if (collision_controller::intersects(player, mushroom) && mushroom.is_active){
    auto& points = mushroom.points_generator.item();
    points.set_active("+1 HP", mushroom.position);

    mushroom.disappear();

    level.stats.hp++;
  }

  run_controller_base(mushroom, level);
}

static auto run_red_mushroom_controller(MushroomState& mushroom, LevelState& level){
  auto& player = level.player;
  if (player.is_dead) return;

  if (collision_controller::intersects(player, mushroom) && mushroom.is_active){
    auto& points = mushroom.points_generator.item();
    points.set_active(mushroom.reward_for_killing, mushroom.position);
  
    mushroom.disappear();

    if (player.growth == PlayerState::Growth::Small){
      player.is_growing_up = true;
    }
    level.stats.score += mushroom.reward_for_killing;
  }

  run_controller_base(mushroom, level);
}

} //namespace mario::mushroom_controller
