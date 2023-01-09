#pragma once

#include "States/QBlockState.hpp"
#include "States/EntityState.hpp"
#include "Controllers/CollisionController.hpp"
#include "Controllers/BounceController.hpp"
#include "Controllers/PlayerController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "Util.hpp"
#include "config.hpp"
#include "res/textures.hpp"

inline auto q_block_points_controller(PointsParticlesState& points, const PlayerState& player){
  points_particles_controller(points);

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

inline auto q_block_controller(QBlockState& block, const PlayerState& player, StatsState& stats){
  if (player_hit_block_above(player, block) && !player.is_dead && block.bounce_state.can_bounce){
    bounce::start(block);

    block.texture = &textures::null_block;
    --block.bounce_state.bounces_count;

    if (block.bounce_state.bounces_count == 0){
      block.bounce_state.can_bounce = false;
      block.texture = &textures::null_block;
    }
  }

  for (auto& p : block.points_manager.points){
    q_block_points_controller(p, player);
  }
}

