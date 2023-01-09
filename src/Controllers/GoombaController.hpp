#pragma once

#include "Controllers/EntityController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "States/EntityState.hpp"
#include "States/LevelState.hpp"
#include "res/textures.hpp"
#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

static auto goomba_controller(GoombaState& goomba, const std::array<Texture, 2>& walk_textures){
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

static auto normal_goomba_controller(GoombaState& goomba, LevelState& level){
  entity_gravity(goomba, level);
  entity_movement(goomba, level);
  entity_turn_around(goomba, config::GoombaWalkSpeed);
  goomba_controller(goomba, textures::goomba_walk);
}

static auto red_goomba_controller(GoombaState& goomba, LevelState& level){
  entity_gravity(goomba, level);
  entity_turn_around(goomba, config::GoombaWalkSpeed);
  entity_movement(goomba, level);
  goomba_controller(goomba, textures::red_goomba_walk);
}

namespace goomba{
  static auto set_dead(EntityState& goomba, Texture* dead_texture){
    goomba.is_dead = true;
    goomba.current_texture = dead_texture;
  }

  static auto normal_set_dead(EntityState& goomba){
    set_dead(goomba, &textures::goomba_dead);
  }

  static auto red_set_dead(EntityState& goomba){
    set_dead(goomba, &textures::red_goomba_dead);
  }
}
