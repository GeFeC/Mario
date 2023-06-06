#pragma once

#include "States/EntityState.hpp"
#include "States/PointsParticlesState.hpp"
#include "Util/Util.hpp"
#include "Util/Generator.hpp"

#include <algorithm>

struct MonsterState : EntityState{
  inline static constexpr auto BounceDiePower = -20.f;
  inline static constexpr auto BouncePower = -15.f;
  inline static constexpr auto EdgeDetectionOffset = BlockBase::Size / 3.f;

  int reward_for_killing = 0;
  int walk_speed = 0.f;

  //When used with q_block
  float push_offset = 0.f;
  bool should_be_pushed_out = false;
  bool is_in_q_block = false;

  util::Generator<PointsParticlesState> points_generator;

  auto set_direction(Direction direction){
    EntityState::set_direction(direction, walk_speed);
  }

  auto spawn_points(int multiplier = 1){
    points_generator.make_item_if_needed();
    points_generator.item().set_active(reward_for_killing * multiplier, position);
  }
};

struct ShellMonsterState : MonsterState{
  bool in_shell = false;
  float shell_speed = 0.f;
  float shell_push_cooldown = 0.f;
  float shell_height = 0.f;
};

