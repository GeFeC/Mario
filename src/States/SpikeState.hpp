#pragma once

#include "States/MonsterState.hpp"

struct SpikeState : MonsterState{
  static constexpr auto WalkSpeed = 2.f;

  static auto make(const glm::vec2& position, Direction direction = DirectionLeft){
    auto spike = SpikeState();
    spike.position = position;
    spike.size = glm::vec2(config::BlockSize);
    spike.walk_speed = 2.f;
    spike.set_direction(direction);
    spike.current_texture = &textures::spike_walk[0];
    spike.texture_flip = Drawable::Flip::UseFlip;
    spike.can_be_stomped = false;

    return spike;
  }
};
