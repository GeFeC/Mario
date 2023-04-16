#pragma once

#include "States/BlockState.hpp"
#include "States/PointsParticlesState.hpp"
#include "Util/Generator.hpp"

struct QBlockState : BouncingBlockState, BlinkingBlockBase{
  static constexpr auto RewardInPoints = 200;
  util::Generator<PointsParticlesState> points_generator;

  QBlockState(const glm::vec2& position){
    this->position = position * BlockBase::Size;
    this->texture = &textures::q_block[0];
  }
};

