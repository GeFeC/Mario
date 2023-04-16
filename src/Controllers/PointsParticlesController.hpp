#pragma once

#include "States/PointsParticlesState.hpp"
#include "Window.hpp"
#include "Util/Util.hpp"

static auto points_particles_controller(PointsParticlesState& points){
  if (points.is_active && points.offset < PointsParticlesState::MaxOffset){
    const auto current_step = window::delta_time * 120.f; 

    points.text.set_position(points.text.get_position() - glm::vec2(0.f, current_step));
    points.offset += current_step;
  }
  else if (points.offset >= PointsParticlesState::MaxOffset){
    points.is_active = false;
    points.text.is_visible = false;
    points.offset = 0.f;
    points.text.position.y = util::BigValue;
  }
}

