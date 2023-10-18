#pragma once

#include "States/EntityState.hpp"
#include "States/BlockState.hpp"
#include "States/LevelState.hpp"

#include "Controllers/CollisionController.hpp"

#include "Util/Util.hpp"

namespace mario{

static auto run_controller(CoinBlockState& coin, LevelState& level){
  coin.texture = &textures::coin[level.blink_state];

  static constexpr auto CollisionPadding = 5;
  auto& player = level.player;

  if (collision_controller::intersects(player, coin, CollisionPadding)){
    coin.position.y = util::BigValue;
    ++level.stats.coins;
  }
}

static auto run_controller(PurpleCoinBlockState& coin, LevelState& level){
  coin.texture = &textures::purple_coin[level.blink_state];

  static constexpr auto CollisionPadding = 5;
  auto& player = level.player;

  if (collision_controller::intersects(player, coin, CollisionPadding)){
    coin.position.y = util::BigValue;

    level.stats.move_direction.toggle();
    player.vertical_flip.toggle();
    player.gravity_flip.toggle();
    player.gravity = -player.gravity;
  }
}

} //namespace mario
