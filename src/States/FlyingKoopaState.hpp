#pragma once

#include "States/KoopaState.hpp"

namespace mario{

struct FlyingKoopaState : KoopaState{
  glm::vec2 initial_position;
  glm::vec2 movement_axis;

  float start_time = 0.f;
  bool has_wings = true;

  static auto make(const glm::vec2& position, const glm::vec2& axis){
    auto koopa = KoopaState::make<FlyingKoopaState>(position);

    koopa.initial_position = position * BlockBase::Size;
    koopa.movement_axis = axis;

    return koopa;
  }

  static auto make_green(const glm::vec2& position, const glm::vec2& axis){
    auto koopa = make(position, axis);
    koopa.type = Type::Green;

    koopa.current_texture = &textures::green_flying_koopa_walk[0];
    return koopa;
  }

  static auto make_red(const glm::vec2& position, const glm::vec2& axis){
    auto koopa = make(position, axis);
    koopa.type = Type::Red;

    koopa.current_texture = &textures::red_flying_koopa_walk[0];
    koopa.fall_from_edge = false;
    return koopa;
  }

  static auto make_purple(const glm::vec2& position, const glm::vec2& axis){
    auto koopa = make(position, axis);
    koopa.type = Type::Purple;

    koopa.fall_from_edge = false;
    koopa.current_texture = &textures::purple_flying_koopa_walk[0];
    return koopa;
  }
};

} //namespace mario
