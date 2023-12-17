#pragma once

#include "States/MonsterState.hpp"

namespace mario{

struct KoopaState : ShellMonsterState{
  enum class Type{
    Green, Red, Purple
  } type;

  template<typename Entity>
  static auto make(const glm::vec2& position){
    auto koopa = Entity();
    koopa.position = (position + glm::vec2(0.f, -0.5f)) * BlockBase::Size;

    koopa.size = glm::vec2(BlockBase::Size, BlockBase::Size * 1.5f);
    koopa.walk_speed = 3.f;
    koopa.shell_speed = 12.f;
    koopa.texture_flip = util::Flip::flip();
    koopa.reward_for_killing = 100.f;
    koopa.shell_height = BlockBase::Size * 7.f / 8.f;

    return koopa;
  }

  static auto make_green(const glm::vec2& position){
    auto koopa = make<KoopaState>(position);
    koopa.type = Type::Green;

    koopa.current_texture = &textures::green_koopa_walk[0];

    return koopa;
  }

  static auto make_red(const glm::vec2& position){
    auto koopa = make<KoopaState>(position);
    koopa.type = Type::Red;

    koopa.current_texture = &textures::red_koopa_walk[0];
    koopa.fall_from_edge = false;

    return koopa;
  }

  static auto make_purple(const glm::vec2& position){
    auto koopa = make<KoopaState>(position);
    koopa.type = Type::Purple;

    koopa.current_texture = &textures::purple_koopa_walk[0];
    koopa.fall_from_edge = false;

    return koopa;
  }
};

} //namespace mario
