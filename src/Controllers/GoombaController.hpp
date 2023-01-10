#pragma once

#include "Controllers/EntityController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "States/EntityState.hpp"
#include "States/LevelState.hpp"
#include "res/textures.hpp"
#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

static auto goomba_controller(GoombaState& goomba, LevelState& level, const std::array<Texture, 2>& walk_textures){
  entity_gravity(goomba, level);
  entity_movement(goomba, level);
  entity_turn_around(goomba, config::GoombaWalkSpeed);

  for (auto& p : goomba.points_manager.points){
    points_particles_controller(p);
  }

  if (goomba.is_dead){
    goomba.death_delay -= window::delta_time;

    if (goomba.death_delay <= 0.f){
      goomba.is_visible = false;
    }

    return;
  } 

  const auto counter = static_cast<int>(glfwGetTime() * 8.f);
  goomba.current_texture = &walk_textures[counter % 2];
}

static auto goomba_set_dead(EntityState& goomba, Texture* dead_texture){
  goomba.is_dead = true;
  goomba.current_texture = dead_texture;
}
