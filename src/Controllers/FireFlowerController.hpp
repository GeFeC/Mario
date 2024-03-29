#pragma once

#include "States/LevelState.hpp"

#include "Controllers/PointsParticlesController.hpp"
#include "Controllers/CollisionController.hpp"

namespace mario::fire_flower_controller{

static auto run(FireFlowerState& flower, LevelState& level){
  for (auto& p : flower.points_generator.items){
    points_particles_controller::run(p);
  }

  //Interaction with player
  auto& player = level.player;
  if (collision_controller::intersects(player, flower) && flower.is_visible){
		flower.points_generator.make_item_if_needed();
    flower.points_generator.item().set_active(
      FireFlowerState::RewardForEating,
      flower.position
    );
    level.stats.score += FireFlowerState::RewardForEating;
  
    flower.is_visible = false;
    flower.position.y = util::BigValue;

    const auto is_player_big = player.growth == PlayerState::Growth::Big;
    if (is_player_big){
      player.is_changing_to_fire = true;
    }
    else{
      player.is_growing_up = true;
    }

		sounds::sounds[sounds::MushroomEat].play();

  }
}

} //namespace mario::fire_flower_controller
