#pragma once

#include "States/EntityState.hpp"
#include "States/BlockState.hpp"
#include "States/LevelState.hpp"

#include "Controllers/CollisionController.hpp"

#include "Util.hpp"

inline auto coin_controller(BlockState& coin, EntityState& player, StatsState& stats){
  static constexpr auto CollisionPadding = 5;

  const auto hovers_in_x = player.position.x - coin.position.x
    | util::in_range(-player.size.x + CollisionPadding, coin.size.x - CollisionPadding); 

  const auto hovers_in_y = player.position.y - coin.position.y 
    | util::in_range(-player.size.y + CollisionPadding, coin.size.y - CollisionPadding); 

  static constexpr auto BigValue = 10000;
  if (hovers_in_x && hovers_in_y){
    coin.position.y = BigValue;
    ++stats.coins;
  }
}

