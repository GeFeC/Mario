#pragma once

#include "Controllers/EntityController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "States/MonsterState.hpp"
#include "States/LevelState.hpp"
#include <array>

static auto spike_controller(
    SpikeState& spike, 
    LevelState& level, 
    const std::array<Texture, 2>& walk_textures,
    int speed = config::GoombaWalkSpeed
){
  entity_gravity(spike, level);
  entity_movement(spike, level);
  entity_turn_around(spike, speed);

  for (auto& p : spike.points_manager.points){
    points_particles_controller(p);
  }

  const auto counter = static_cast<int>(glfwGetTime() * 8.f);
  spike.current_texture = &walk_textures[counter % 2];
}
