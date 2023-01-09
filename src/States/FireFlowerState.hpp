#pragma once

#include "States/BlockState.hpp"
#include "States/PointsParticlesState.hpp"

struct FireFlowerState : BlockBase{
  float offset = 0.f;
  bool should_be_pushed_out = false;
  int points_index = -1;

  PointsParticlesManager points_manager;

  FireFlowerState(const glm::vec2& position){
    this->position = position * 60.f;
    texture = &textures::fire_flower[0];
  }
};

