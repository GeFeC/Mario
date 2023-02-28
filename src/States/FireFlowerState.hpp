#pragma once

#include "States/BlockState.hpp"
#include "States/PointsParticlesState.hpp"
#include "Util/Generator.hpp"

struct FireFlowerState : BlockBase{
  static constexpr auto RewardForEating = 1000;

  float offset = 0.f;
  bool should_be_pushed_out = false;
  int points_index = -1;

  util::Generator<PointsParticlesState> points_generator;

  FireFlowerState(const glm::vec2& position){
    this->position = position * 60.f;
    texture = &textures::fire_flower[0];
  }
};

