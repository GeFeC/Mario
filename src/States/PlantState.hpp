#pragma once

#include "MonsterState.hpp"

struct PlantState : MonsterState{
  static constexpr auto StartCooldown = 1.f;
  static constexpr auto MaxOffset = config::BlockSize * 11 / 8;
  float offset = 0.f;
  float cooldown = 0.f;

  enum class Direction{
    GoingUp,
    GoingDown
  } direction = Direction::GoingUp;
};
