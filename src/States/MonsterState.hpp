#pragma once

#include "States/EntityState.hpp"
#include "States/PointsParticlesState.hpp"

#include <algorithm>

struct MonsterState : EntityState{
  PointsParticlesManager points_manager;
};

struct GoombaState : MonsterState{};
struct SpikeState : MonsterState{};

struct MushroomState : MonsterState{
  float offset = 0.f;
  bool should_be_pushed_out = false;
};

struct ShellMonsterState : MonsterState{
  bool in_shell = false;
  float shell_push_cooldown = 0.f;
  float current_walk_speed = 0;
};

struct KoopaState : ShellMonsterState{
  KoopaState(){
    current_walk_speed = config::KoopaWalkSpeed;
  }
};

struct FlyingKoopaState : KoopaState{
  bool has_wings = true;
  FlyingKoopaState() : KoopaState() {}
};

struct BeetleState : ShellMonsterState{
  BeetleState(){
    current_walk_speed = config::BeetleWalkSpeed;
  }
};
