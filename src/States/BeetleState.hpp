#pragma once

#include "States/MonsterState.hpp"

struct BeetleState : ShellMonsterState{
  static auto make(const glm::vec2& position, Direction direction = DirectionLeft){
    auto beetle = BeetleState();
    beetle.position = position * config::BlockSize;
    beetle.size = glm::vec2(config::BlockSize);
    beetle.walk_speed = 3.f;
    beetle.shell_speed = 12.f;
    beetle.set_direction(direction);
    beetle.current_texture = &textures::beetle_walk[0];
    beetle.texture_flip = Drawable::Flip::UseFlip;
    beetle.shell_height = config::BlockSize;
    beetle.reward_for_killing = 100;

    return beetle;
  }
};
