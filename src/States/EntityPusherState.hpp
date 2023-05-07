#pragma once

#include "States/GoombaState.hpp"
#include "States/MushroomState.hpp"

template<typename Entity>
struct EntityPusherState{
  static constexpr auto ContainsEntity = true;

  Entity entity;
};

using GoombaPusherState = EntityPusherState<GoombaState>;
using MushroomPusherState = EntityPusherState<MushroomState>;
