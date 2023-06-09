#pragma once

#include "States/BlockState.hpp"
#include "States/PointsParticlesState.hpp"
#include "Util/Generator.hpp"

namespace mario{

inline constexpr auto QBlockReward = 200;

template<typename Pusher>
struct QBlockState : BouncingBlockState, BlinkingBlockBase{
  //Contents pusher
  using PusherType = Pusher;
  Pusher pusher;

  QBlockState(const glm::vec2& position){
    this->position = position * BlockBase::Size;
    this->texture = &textures::q_block[0];
  }
};

} //namespace mario
