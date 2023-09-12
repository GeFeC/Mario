#pragma once

#include "States/MonsterState.hpp"

namespace mario{

struct BeetleState : ShellMonsterState{
  static constexpr auto DefaultWalkSpeed = 3.f;

  static auto make(const glm::vec2& position){
    auto beetle = BeetleState();
    beetle.position = position * BlockBase::Size;
    beetle.size = glm::vec2(BlockBase::Size);
    beetle.walk_speed = DefaultWalkSpeed;
    beetle.shell_speed = 12.f;
    beetle.current_texture = &textures::beetle_walk[0];
    beetle.texture_flip = util::Flip::flip();
    beetle.shell_height = BlockBase::Size;
    beetle.reward_for_killing = 100;

    return beetle;
  }
};

} //namespace mario
