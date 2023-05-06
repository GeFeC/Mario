#pragma once

#include "States/CoinPusherState.hpp"
#include "States/LevelState.hpp"

#include "Controllers/BounceController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "States/QBlockState.hpp"

static auto pusher_push_out(CoinPusherState& pusher, LevelState& level){
  auto& coin = pusher.active_coin();
  auto block_pos = coin.position;
  
  coin.is_visible = true;
  bounce::start(coin);

  ++level.stats.coins;
  level.stats.score += QBlockReward;
  ++pusher.active_coin_index;

  //Points
  pusher.points_generator.make_item_if_needed();
  auto& points = pusher.points_generator.item();
  points.set_active(QBlockReward, block_pos);
}

static auto pusher_controller(CoinPusherState& pusher, const LevelState& level){
  for (auto& points : pusher.points_generator.items){
    points_particles_controller(points);
  }

  for (auto& coin : pusher.coins){
    coin.texture = &textures::spinning_coin[level.coin_spin_counter.int_value()];

    bounce_controller(coin);

    if (!coin.bounce_state.is_bouncing){
      coin.is_visible = false;
    }
  }
}
