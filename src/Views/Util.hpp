#pragma once

#include "Renderer/Renderer.hpp"
#include "Controllers/CollisionController.hpp"
#include "States/PointsParticlesState.hpp"

namespace mario::views{

static auto is_component_on_screen(const collision_controller::Rect& component, const glm::vec2& offset){
  using config::FrameBufferSize;

  const auto& position = component.position;
  const auto& size = component.size;

  const auto is_x = position.x - offset.x == util::in_range(-size.x, FrameBufferSize.x);
  const auto is_y = position.y - offset.y == util::in_range(-size.y, FrameBufferSize.y);

  return is_x && is_y;
}

static auto render_points_particles(const std::vector<PointsParticlesState>& points, const glm::vec2& offset){
  for (const auto& point : points){
    renderer::print(point.text, offset);
  }
}


} //namespace mario::views
