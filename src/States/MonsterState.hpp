#pragma once

#include "States/EntityState.hpp"
#include "States/PointsParticlesState.hpp"

#include <algorithm>

struct MonsterState : EntityState{
  PointsParticlesManager points_manager;
};

struct GoombaState : MonsterState{};

struct MushroomState : MonsterState{
  float offset = 0.f;
  bool should_be_pushed_out = false;
};
