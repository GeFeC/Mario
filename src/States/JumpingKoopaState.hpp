#pragma once

#include "States/KoopaState.hpp"

struct JumpingKoopaState : KoopaState{
  static constexpr auto JumpForce = -20.f;
  bool has_wings = true;

  static auto make_green(const glm::vec2& position, Direction direction = DirectionLeft){
    auto koopa = KoopaState::make<JumpingKoopaState>(position, direction);
    koopa.current_texture = &textures::green_flying_koopa_walk[0];
    koopa.gravity_boost = 0.7f;

    return koopa;
  }

  static auto make_red(const glm::vec2& position, Direction direction = DirectionLeft){
    auto koopa = KoopaState::make<JumpingKoopaState>(position, direction);
    koopa.current_texture = &textures::red_flying_koopa_walk[0];
    koopa.gravity_boost = 0.7f;

    return koopa;
  }

  static auto make_purple(const glm::vec2& position, Direction direction = DirectionLeft){
    auto koopa = KoopaState::make<JumpingKoopaState>(position, direction);
    koopa.current_texture = &textures::purple_flying_koopa_walk[0];
    koopa.gravity_boost = 0.7f;

    return koopa;
  }
};
