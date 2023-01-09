#pragma once

#include "States/BlockState.hpp"
#include "States/PointsParticlesState.hpp"

struct QBlockState : BouncingBlockState, BlinkingBlockBase{
  PointsParticlesManager points_manager;

  QBlockState(const glm::vec2& position){
    this->position = position * 60.f;
    this->texture = &textures::q_block[0];
  }
};

