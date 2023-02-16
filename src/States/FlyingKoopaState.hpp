#pragma once

#include "States/KoopaState.hpp"

struct FlyingKoopaState : KoopaState{
  bool has_wings = true;
  glm::vec2 initial_position;
  glm::vec2 movement_axis;
  float start_time = 0.f;

  static auto make_green(const glm::vec2& position, const glm::vec2& axis, Direction direction = DirectionLeft){
    auto koopa = KoopaState::make<FlyingKoopaState>(position, direction);
    koopa.current_texture = &textures::green_flying_koopa_walk[0];
    koopa.initial_position = position * config::BlockSize;
    koopa.movement_axis = axis;

    return koopa;
  }

  static auto make_red(const glm::vec2& position, const glm::vec2& axis, Direction direction = DirectionLeft){
    auto koopa = KoopaState::make<FlyingKoopaState>(position, direction);
    koopa.current_texture = &textures::red_flying_koopa_walk[0];
    koopa.fall_from_edge = false;
    koopa.initial_position = position * config::BlockSize;
    koopa.movement_axis = axis;

    return koopa;
  }
};
