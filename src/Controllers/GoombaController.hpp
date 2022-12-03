#pragma once

#include "States/EntityState.hpp"
#include "res/textures.hpp"
#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

static auto goomba_controller(EntityState& goomba, const std::array<Texture, 2>& walk_textures){
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

static auto normal_goomba_controller(EntityState& goomba){
  goomba_controller(goomba, textures::goomba_walk);
}

static auto red_goomba_controller(EntityState& goomba){
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
