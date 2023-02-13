#pragma once

#include "MonsterState.hpp"

struct PlantState : MonsterState{
  static constexpr auto StartCooldown = 1.f;
  static constexpr auto MaxOffset = config::BlockSize * 11 / 8;
  float offset = 0.f;
  float cooldown = 0.f;

  enum class Direction{
    GoingUp,
    GoingDown
  } direction = Direction::GoingUp;

  static auto make(const glm::vec2& position){
    auto plant = PlantState();
    plant.position = position * config::BlockSize;
    plant.size = glm::vec2(config::BlockSize, config::BlockSize * 11 / 8);
    plant.current_texture = &textures::plant[0];
    plant.can_be_stomped = false;

    return plant;
  }
};
