#pragma once

#include "States/BlockState.hpp"
#include "States/EntityState.hpp"
#include "Controllers/PlayerController.hpp"
#include "Controllers/BounceController.hpp"

inline auto spinning_coin_controller(SpinningCoinState& coin, LevelState& level){
  coin.texture = &textures::spinning_coin[LevelState::coin_spin_counter.int_value()];

  bounce_controller(coin);

  auto& player = level.player;
  if (player_hit_block_above(player, coin) && coin.bounce_state.can_bounce){
    --coin.bounce_state.hits_required_to_bounce;
    coin.bounce_state.can_bounce = false;

    if (coin.bounce_state.hits_required_to_bounce > 0){
      return;
    }
    
    coin.is_visible = true;
    bounce::start(coin);

    ++level.stats.coins;
    level.stats.score += QBlockState::RewardInPoints;
  }

  if (!coin.bounce_state.is_bouncing && !coin.bounce_state.can_bounce){
    coin.is_visible = false;
  }

  if (!player_hit_block_above(player, coin) && coin.bounce_state.hits_required_to_bounce > 0){
    coin.bounce_state.can_bounce = true;
  }
}
