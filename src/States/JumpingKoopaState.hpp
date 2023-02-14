#pragma once

#include "States/KoopaState.hpp"

struct JumpingKoopaState : KoopaState{
  bool has_wings = true;

  static auto make_green(const glm::vec2& position, Direction direction = DirectionLeft){
    auto koopa = KoopaState::make<JumpingKoopaState>(position, direction);
    koopa.current_texture = &textures::green_flying_koopa_walk[0];

    return koopa;
  }

  static auto make_red(const glm::vec2& position, Direction direction = DirectionLeft){
    auto koopa = KoopaState::make<JumpingKoopaState>(position, direction);
    koopa.current_texture = &textures::red_flying_koopa_walk[0];
    koopa.fall_from_edge = false;

    return koopa;
  }
};
