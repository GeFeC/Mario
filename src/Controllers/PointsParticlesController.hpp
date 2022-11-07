#pragma once

#include "States/PointsParticlesState.hpp"
#include "Window.hpp"

inline auto points_particles_controller(PointsParticlesState& points){
  if (points.is_active && points.offset < 180.f){
    const auto current_step = window::delta_time * 120.f; 

    points.text.set_position(points.text.get_position() - glm::vec2(0.f, current_step));
    points.offset += current_step;
  }
  else{
    points.is_active = false;
    points.text.is_visible = false;
  }
}
