#pragma once

#include "States/MonsterState.hpp"

namespace mario{

struct SpikeState : MonsterState{
  static constexpr auto WalkSpeed = 2.f;

  bool can_move = true;

  static auto make(const glm::vec2& position){
    auto spike = SpikeState();
    spike.position = position * BlockBase::Size;
    spike.size = glm::vec2(BlockBase::Size);
    spike.walk_speed = 3.f;
    spike.current_texture = &textures::spike_walk[0];
    spike.texture_flip = util::Flip::flip();
    spike.can_be_stomped = false;
    spike.reward_for_killing = 100;

    return spike;
  }
};

} //namespace mario
