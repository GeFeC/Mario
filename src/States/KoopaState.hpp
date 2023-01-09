#pragma once

#include "States/MonsterState.hpp"
#include "States/PointsParticlesState.hpp"

struct KoopaState : MonsterState{
  bool in_shell = false;
  float shell_push_cooldown = 0.f;
  float current_walk_speed = config::KoopaWalkSpeed;
};
