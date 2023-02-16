#pragma once

#include "States/FireballState.hpp"
#include <vector>

struct FireBarState{
  static constexpr auto RotationSpeed = 2.f;

  glm::vec2 position;
  std::vector<FireballState> fireballs;

  FireBarState(const glm::vec2& position, int length) : fireballs(length) {
    const auto fireball_size = fireballs[0].size.x;

    //Center of the block
    using config::BlockSize;
    this->position = position * BlockSize + glm::vec2(BlockSize / 2) - glm::vec2(fireball_size / 2);

    for (auto& fireball : fireballs){
      fireball.is_visible = true;
    }
  }
};
