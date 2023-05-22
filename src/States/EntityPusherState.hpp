#pragma once

#include "States/GoombaState.hpp"
#include "States/JumpingKoopaState.hpp"
#include "States/FlyingKoopaState.hpp"
#include "States/MushroomState.hpp"

template<typename Entity>
struct EntityPusherState{
  static constexpr auto ContainsEntity = true;

  float push_distance_multiplier = 1.f;
  Entity entity;
};

using GoombaPusherState = EntityPusherState<GoombaState>;
using KoopaPusherState = EntityPusherState<KoopaState>;
using JumpingKoopaPusherState = EntityPusherState<JumpingKoopaState>;
using FlyingKoopaPusherState = EntityPusherState<FlyingKoopaState>;
using MushroomPusherState = EntityPusherState<MushroomState>;
