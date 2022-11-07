#pragma once

#include "States/EntityState.hpp"
#include "res/textures.hpp"
#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

static auto goomba_controller(EntityState& goomba){
  if (goomba.is_dead){
    goomba.death_delay -= window::delta_time;

    if (goomba.death_delay <= 0.f){
      goomba.is_visible = false;
    }

    return;
  } 

  const auto counter = static_cast<int>(glfwGetTime() * 8.f);

  goomba.current_texture = &textures::goomba_walk[counter % 2];
}

namespace goomba{
  static auto on_dead(EntityState& goomba){
    goomba.is_dead = true;
    goomba.current_texture = &textures::goomba_dead;
  }
}
