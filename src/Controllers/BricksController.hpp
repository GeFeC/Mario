#pragma once

#include "States/BlockState.hpp"
#include "States/EntityState.hpp"
#include "States/LevelState.hpp"

#include "Controllers/PlayerController.hpp"
#include "Controllers/BounceController.hpp"
#include "Controllers/EntityController.hpp"

static auto bricks_controller(BricksBlockState& block, PlayerState& player, LevelState& level){
  bounce_controller(block);
  if (player_hit_block_above(player, block) && block.is_solid){
    if (player.growth == PlayerState::Growth::Big){
      level.get_hitbox_grid_element(block.position / config::BlockSize) = 0;

      block.is_visible = false;
      block.is_solid = false;
      block.are_particles_active = true;

      player.gravity = 0;

      level.blocks.normal[block.hitbox_block_index].is_solid = false;

      //Particles init gravity and direction:

      for (auto& particle : block.particles){
        particle.is_active = true;
        particle.is_visible = true;
        particle.gravity = util::random_value(-20, -14);

        const auto direction = util::random_value(0, 1) 
          ? EntityState::DirectionRight 
          : EntityState::DirectionLeft;

        particle.set_direction(direction, util::random_value(2, 5));
      }
    }

    bounce::start(block);
  }

  //Particles:

  if (block.are_particles_active){
    for (auto& particle : block.particles){
      entity_gravity(particle, level);
      entity_movement(particle, level);
    }    
  } 
}
