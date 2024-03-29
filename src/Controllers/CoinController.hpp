#pragma once

#include "States/BlockState.hpp"
#include "States/LevelState.hpp"

#include "Controllers/CollisionController.hpp"

#include "Util/Util.hpp"

#include "res/sounds.hpp"

namespace mario{

static auto run_controller(CoinBlockState& coin, LevelState& level){
  coin.texture = &textures::coin[level.blink_state];

  static constexpr auto CollisionPadding = 5;
  auto& player = level.player;

  if (player.is_dead) return;

  if (collision_controller::intersects(player, coin, CollisionPadding)){
    coin.position.y = util::BigValue;
    ++level.stats.coins;

		sounds::sounds[sounds::Coin].play();
  }
}

static auto run_controller(PurpleCoinBlockState& coin, LevelState& level){
  coin.texture = &textures::purple_coin[level.blink_state];

  static constexpr auto CollisionPadding = 5;
  auto& player = level.player;

  if (player.is_dead) return;

  if (collision_controller::intersects(player, coin, CollisionPadding)){
    coin.position.y = util::BigValue;

    level.stats.move_direction.toggle();
    player.vertical_flip.toggle();
    player.gravity_flip.toggle();
    player.gravity = -player.gravity;

		sounds::sounds[sounds::Jump].play();
  }
}

} //namespace mario
