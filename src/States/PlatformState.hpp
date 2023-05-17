#pragma once
#include "States/BlockState.hpp"
#include "States/EntityState.hpp"
#include <vector>

struct PlatformState{ 
  inline static constexpr auto DefaultWidth = 8;
  inline static constexpr auto ElementSize = BlockBase::Size / 2.f;
  inline static constexpr auto Speed = 800;

  std::vector<EntityState*> passengers;

  glm::vec2 position;
  glm::vec2 initial_position;
  glm::vec2 transport_distance;
  int width = DefaultWidth;
  float timer = 0.f;
  bool is_active = false;

  PlatformState(const glm::vec2& position, const glm::vec2& transport_distance, int width = DefaultWidth)
  : position(position * BlockBase::Size), 
    initial_position(this->position), 
    transport_distance(transport_distance * BlockBase::Size),
    width(width) {
    
    timer = initial_timer_value();
  }

  auto size() const{
    return glm::vec2(
      width * ElementSize,
      ElementSize
    );
  }

  auto move_distance() const{
    return glm::sqrt(
      glm::pow(transport_distance.x, 2) +
      glm::pow(transport_distance.y, 2)
    );
  }

  auto initial_timer_value() const -> double{
    return glm::radians(-90.f * move_distance() / Speed);
  }
};

struct LoopedPlatformState : PlatformState{
  LoopedPlatformState(
      const glm::vec2& position, 
      const glm::vec2& transport_distance, 
      int width = DefaultWidth
  ) 
    : PlatformState(position, transport_distance, width) {}
};
