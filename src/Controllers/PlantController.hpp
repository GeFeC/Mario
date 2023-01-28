#pragma once

#include "States/MonsterState.hpp"
#include "res/textures.hpp"
#include "Window.hpp"

#include <GLFW/glfw3.h>

static auto plant_controller(PlantState& plant){
  const auto counter = static_cast<int>(glfwGetTime() * 8.f);
  plant.current_texture = &textures::plant[counter % 2];

  const auto offset = window::delta_time * 50.f;

  using Direction = PlantState::Direction;
  if (plant.cooldown == 0.f){
    if (plant.direction == Direction::GoingUp && plant.offset < PlantState::MaxOffset){
      plant.offset += offset;
      plant.position.y -= offset;
      return;
    }
    else{
      if (plant.direction == Direction::GoingUp){
        plant.cooldown = PlantState::StartCooldown;
      }

      plant.direction = Direction::GoingDown;
    }

    if (plant.direction == Direction::GoingDown && plant.offset > 0.f){
      plant.offset -= offset;
      plant.position.y += offset;
      return;
    }
    else{
      if (plant.direction == Direction::GoingDown){
        plant.cooldown = PlantState::StartCooldown;
      }

      plant.direction = Direction::GoingUp;
    }
  }
  plant.cooldown = std::max(plant.cooldown - window::delta_time, 0.f);
}
