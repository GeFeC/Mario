#pragma once

#include "States/PlantState.hpp"
#include "States/FireballState.hpp"

#include "Util/LoopedCounter.hpp"
#include "Util/Generator.hpp"

#include <vector>

namespace mario{

struct BlackPlantState : PlantState{
  static constexpr auto FireballGravity = -20.f;

  util::Generator<FireballState> fireball_generator;
  util::LoopedCounter shot_counter = util::LoopedCounter(util::BigValue, 10.f, 1);
  EntityState::Direction fireball_direction = DirectionLeft;

  static auto make(const glm::vec2& position){
    auto plant = PlantState::make<BlackPlantState>(position);

    plant.cooldown_duration = 1.5f;
    plant.speed = 300.f;
    plant.is_active = false;

    return plant;
  }
};

} //namespace mario
