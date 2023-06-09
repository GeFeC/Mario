#pragma once

#include "States/FireFlowerState.hpp"

namespace mario{

struct FireFlowerPusherState{
  static constexpr auto ContainsEntity = false;

  FireFlowerState fire_flower;
};

} //namespace mario
