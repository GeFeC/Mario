#pragma once

#include "States/FireFlowerState.hpp"

struct FireFlowerPusherState{
  static constexpr auto ContainsEntity = false;

  FireFlowerState fire_flower;
};
