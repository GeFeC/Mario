#pragma once

#include "Util/Util.hpp"
#include "Util/Interval.hpp"
#include "config.hpp"

namespace mario::collision_controller{

struct CollisionState{
  float distance_above = 0.f;
  float distance_below = 0.f;
  float distance_left = 0.f;
  float distance_right = 0.f;
};

struct Rect{
  glm::vec2 position;
  glm::vec2 size;

  Rect() = default;

  template<typename T>
  Rect(const T& t){
    position = t.position;
    size = t.size;
  }

  Rect(const glm::vec2& pos, const glm::vec2& size)
  : position(pos), size(size) {}
};


static auto CollisionPadding = 10.f;

static auto intersects_in_x(const Rect& object1, const Rect& object2){
  return object1.position.x - object2.position.x 
    == util::in_range(-object1.size.x + CollisionPadding, object2.size.x - CollisionPadding);
}

static auto intersects_in_y(const Rect& object1, const Rect& object2){
  return object1.position.y - object2.position.y 
    == util::in_range(-object1.size.y + CollisionPadding, object2.size.y - CollisionPadding);
}

static auto intersects(const Rect& object1, const Rect& object2) {
  return 
    intersects_in_x(object1, object2) &&
    intersects_in_y(object1, object2);
}

static auto controller(const Rect& object1, const Rect& object2){
  using util::BigValue;
  auto collision_state = CollisionState{ BigValue, BigValue, BigValue, BigValue };

  if (intersects_in_x(object1, object2)){
    collision_state.distance_below 
      = -(object1.position.y + object1.size.y - object2.position.y);

    collision_state.distance_above 
      = std::abs(object2.position.y + object2.size.y - object1.position.y - CollisionPadding);
  }

  if (intersects_in_y(object1, object2)){
    collision_state.distance_left 
      = (object1.position.x - (object2.size.x + object2.position.x - CollisionPadding + 0.1));

    collision_state.distance_right 
      = (object2.position.x - (object1.size.x + object1.position.x - CollisionPadding + 0.1));
  }

  return collision_state;
}

} //namespace mario::collision_controller
