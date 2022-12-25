#pragma once

#include "States/EntityState.hpp"
#include "States/PlayerState.hpp"
#include "States/LevelState.hpp"
#include "States/BlockState.hpp"

#include "Controllers/CollisionController.hpp"

auto player_controller(PlayerState& player, LevelState& level) -> void;

static auto player_can_hit_block_above(const EntityState& player, const BouncingBlockState& block) -> bool{
  if (player.direction == EntityState::DirectionLeft){
    return player.position.x - block.position.x 
      | util::in_range(0.f, block.size.x - CollisionOffset);
  }

  return player.position.x - block.position.x 
    | util::in_range(-player.size.x + CollisionOffset, 0.f);
}

static auto player_hit_block_above(const EntityState& player, const BouncingBlockState& block) -> bool{
  const auto was_block_hit_by_player = (player.position.y - block.position.y - block.size.y) | util::in_range(-10, -5);

  return 
    player_can_hit_block_above(player, block) && 
    was_block_hit_by_player && 


    !block.bounce_state.is_bouncing;
}

auto player_stomp_on_entity(const EntityState& player, const EntityState& entity) -> bool;
