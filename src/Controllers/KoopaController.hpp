#pragma once

#include "Controllers/EntityController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "States/EntityState.hpp"
#include "States/LevelState.hpp"
#include "res/textures.hpp"
#include "config.hpp"

static auto koopa_controller(KoopaState& koopa, LevelState& level, const std::array<Texture, 2>& walk){
  entity_gravity(koopa, level);
  entity_movement(koopa, level);
  entity_turn_around(koopa, koopa.current_walk_speed);

  for (auto& p : koopa.points_manager.points){
    points_particles_controller(p);
  }

  if (koopa.is_dead || koopa.in_shell) return;

  const auto counter = static_cast<int>(glfwGetTime() * 8.f);
  koopa.current_texture = &walk[counter % 2];
}

static auto koopa_hide_in_shell(KoopaState& koopa, Texture const* dead_texture){
  koopa.in_shell = true;
  koopa.current_texture = dead_texture;
  koopa.acceleration.left = koopa.acceleration.right = 0.f;
  koopa.current_walk_speed = 0.f;

  const auto previous_size = koopa.size.y;
  koopa.size.y = config::BlockSize * 7 / 8;

  koopa.position.y += previous_size - koopa.size.y;
}
