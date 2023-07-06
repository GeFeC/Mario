#pragma once

#include "States/EntityState.hpp"
#include "States/BlockState.hpp"
#include "States/LevelState.hpp"

#include "Controllers/CollisionController.hpp"
#include "PolyControllers.hpp"

#include "Util/Util.hpp"

namespace mario{

static auto run_controller(CoinBlockState& coin, LevelState& level){
  coin.texture = &textures::coin[level.blink_state];

  static constexpr auto CollisionPadding = 5;
  auto& player = level.player;
  
  const auto hovers_in_x = player.position.x - coin.position.x
    == util::in_range(-player.size.x + CollisionPadding, coin.size.x - CollisionPadding); 

  const auto hovers_in_y = player.position.y - coin.position.y 
    == util::in_range(-player.size.y + CollisionPadding, coin.size.y - CollisionPadding); 

  if (hovers_in_x && hovers_in_y){
    coin.position.y = util::BigValue;
    ++level.stats.coins;
  }
}

} //namespace mario
