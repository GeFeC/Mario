#pragma once

#include "States/MonsterState.hpp"

struct BeetleState : ShellMonsterState{
  static constexpr auto DefaultWalkSpeed = 3.f;

  static auto make(const glm::vec2& position, Direction direction = DirectionLeft){
    auto beetle = BeetleState();
    beetle.position = position * BlockBase::Size;
    beetle.size = glm::vec2(BlockBase::Size);
    beetle.walk_speed = DefaultWalkSpeed;
    beetle.shell_speed = 12.f;
    beetle.set_direction(direction);
    beetle.current_texture = &textures::beetle_walk[0];
    beetle.texture_flip = Drawable::Flip::UseFlip;
    beetle.shell_height = BlockBase::Size;
    beetle.reward_for_killing = 100;

    return beetle;
  }
};
