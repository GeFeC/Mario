#pragma once

#include "States/BlockState.hpp"
#include "States/PointsParticlesState.hpp"
#include "Util/Generator.hpp"

inline constexpr auto QBlockReward = 200;

template<typename Pusher>
struct QBlockState : BouncingBlockState, BlinkingBlockBase{
  //Contents pusher
  Pusher pusher;

  QBlockState(const glm::vec2& position){
    this->position = position * BlockBase::Size;
    this->texture = &textures::q_block[0];
  }
};

