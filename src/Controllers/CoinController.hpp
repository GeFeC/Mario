#pragma once

#include "States/EntityState.hpp"
#include "States/BlockState.hpp"
#include "States/LevelState.hpp"

#include "Controllers/CollisionController.hpp"

#include "Util/Util.hpp"

inline auto coin_controller(CoinBlockState& coin, LevelState& level){
  coin.texture = &textures::coin[LevelState::blink_state];

  static constexpr auto CollisionPadding = 5;
  auto& player = level.player;
  
  const auto hovers_in_x = player.position.x - coin.position.x
    == util::in_range(-player.size.x + CollisionPadding, coin.size.x - CollisionPadding); 

  const auto hovers_in_y = player.position.y - coin.position.y 
    == util::in_range(-player.size.y + CollisionPadding, coin.size.y - CollisionPadding); 

  if (hovers_in_x && hovers_in_y){
    coin.position.y = config::BigValue;
    ++level.stats.coins;
  }
}
