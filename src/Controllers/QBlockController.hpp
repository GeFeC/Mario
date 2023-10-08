#pragma once

#include "Controllers/CoinPusherController.hpp"
#include "Controllers/EntityPusherController.hpp"
#include "Controllers/FireFlowerPusherController.hpp"

#include "States/QBlockState.hpp"
#include "States/EntityState.hpp"
#include "Controllers/CollisionController.hpp"
#include "Controllers/BounceController.hpp"
#include "Controllers/PlayerController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "Util/Util.hpp"
#include "config.hpp"
#include "res/textures.hpp"

namespace mario{

template<typename Pusher>
static auto run_controller(QBlockState<Pusher>& block, LevelState& level){
  bounce_controller::run(block);
  pusher_controller::run(block.pusher, level);

  if (block.bounce_state.can_bounce){
    block.texture = &textures::q_block[level.blink_state];
  } 

  auto& player = level.player;
  if (player_controller::did_hit_block_above(player, block) && !player.is_dead && block.bounce_state.can_bounce){
    pusher_controller::push_out(block.pusher, level);

    bounce_controller::bounce(block);
    block.is_visible = true;

    block.texture = &textures::null_block;
    --block.bounce_state.bounces_count;

    if (block.bounce_state.bounces_count == 0){
      block.bounce_state.can_bounce = false;
      block.texture = &textures::null_block;
    }
  }
}

} //namespace mario
