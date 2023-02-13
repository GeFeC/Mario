#pragma once

#include "States/MonsterState.hpp"

struct BeetleState : ShellMonsterState{
  static auto make(const glm::vec2& position, Direction direction = DirectionLeft){
    auto beetle = BeetleState();
    beetle.position = position;
    beetle.size = glm::vec2(config::BlockSize);
    beetle.walk_speed = 2.f;
    beetle.set_direction(direction);
    beetle.current_texture = &textures::beetle_walk[0];
    beetle.texture_flip = Drawable::Flip::UseFlip;
    beetle.shell_height = config::BlockSize;

    return beetle;
  }
};
