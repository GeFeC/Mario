#pragma once

#include "Util/Generator.hpp"

#include "States/PointsParticlesState.hpp"
#include "States/BlockState.hpp"

#include <vector>

namespace mario{

struct CoinPusherState{
  static constexpr auto ContainsEntity = false;

  util::Generator<PointsParticlesState> points_generator;

  std::vector<SpinningCoinState> coins;
  int active_coin_index = 0;

  auto& active_coin(){
    return coins[active_coin_index];
  }
};

} //namespace mario
