#pragma once

#include "States/BlockState.hpp"
#include "States/EntityState.hpp"
#include "Controllers/CollisionController.hpp"
#include "Controllers/BounceController.hpp"
#include "Controllers/PlayerController.hpp"
#include "Util.hpp"
#include "config.hpp"

inline auto q_block_controller(BlockState& block, EntityState& player, StatsState& stats){
  if (player_hit_block_above(player, block) && !player.is_dead){
    bounce::start(block);

    block.texture = &textures::null_block;
    --block.bounce_state.bounces_count;

    if (block.bounce_state.bounces_count == 0){
      block.types = { BlockState::Type::Solid };
    }

    ++stats.coins;
    stats.score += config::RewardForQBlock;
  }
}

inline auto q_block_points_controller(PointsParticlesState& points, const EntityState& player){
  if (player_hit_block_above(player, points.as_block()) && points.can_be_activated){
    --points.hits_required_to_activate;
    points.can_be_activated = false;
    if (points.hits_required_to_activate > 0) return;

    points.text.is_visible = true;
    points.is_active = true;
  }

  if (!player_hit_block_above(player, points.as_block())){
    points.can_be_activated = true;
  }
}
