#pragma once

#include "States/MonsterState.hpp"

struct SpikeState : MonsterState{
  static constexpr auto WalkSpeed = 2.f;

  static auto make(const glm::vec2& position, Direction direction = DirectionLeft){
    auto spike = SpikeState();
    spike.position = position * config::BlockSize;
    spike.size = glm::vec2(config::BlockSize);
    spike.walk_speed = 3.f;
    spike.set_direction(direction);
    spike.current_texture = &textures::spike_walk[0];
    spike.texture_flip = Drawable::Flip::UseFlip;
    spike.can_be_stomped = false;
    spike.reward_for_killing = 100;

    return spike;
  }
};
