#pragma once

#include "States/BlockState.hpp"
#include "States/PointsParticlesState.hpp"

struct QBlockState : BouncingBlockState, BlinkingBlockBase{
  static constexpr auto RewardInPoints = 200;
  PointsParticlesManager points_manager;

  QBlockState(const glm::vec2& position){
    this->position = position * config::BlockSize;
    this->texture = &textures::q_block[0];
  }
};

