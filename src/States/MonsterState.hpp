#pragma once

#include "States/EntityState.hpp"
#include "States/PointsParticlesState.hpp"
#include "Util.hpp"

#include <algorithm>

struct MonsterState : EntityState{
  static constexpr auto BounceDiePower = -20.f;
  static constexpr auto BouncePower = -15.f;

  int reward_for_killing = 0;
  int walk_speed = 0.f;

  PointsParticlesManager points_manager;

  auto set_direction(Direction direction){
    EntityState::set_direction(direction, walk_speed);
  }
};

struct ShellMonsterState : MonsterState{
  bool in_shell = false;
  float shell_speed = 0.f;
  float shell_push_cooldown = 0.f;
  float shell_height = 0.f;
};

