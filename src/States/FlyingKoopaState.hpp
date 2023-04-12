#pragma once

#include "States/KoopaState.hpp"

struct FlyingKoopaState : KoopaState{
  bool has_wings = true;
  glm::vec2 initial_position;
  glm::vec2 movement_axis;
  float start_time = 0.f;

  static auto make(const glm::vec2& position, const glm::vec2& axis){
    auto koopa = KoopaState::make<FlyingKoopaState>(position);
    koopa.initial_position = position * config::BlockSize;
    koopa.movement_axis = axis;

    return koopa;
  }

  static auto make_green(const glm::vec2& position, const glm::vec2& axis){
    auto koopa = make(position, axis);

    koopa.current_texture = &textures::green_flying_koopa_walk[0];
    return koopa;
  }

  static auto make_red(const glm::vec2& position, const glm::vec2& axis){
    auto koopa = make(position, axis);

    koopa.current_texture = &textures::red_flying_koopa_walk[0];
    koopa.fall_from_edge = false;
    return koopa;
  }

  static auto make_purple(const glm::vec2& position, const glm::vec2& axis){
    auto koopa = make(position, axis);

    koopa.fall_from_edge = false;
    koopa.current_texture = &textures::purple_flying_koopa_walk[0];
    return koopa;
  }
};
