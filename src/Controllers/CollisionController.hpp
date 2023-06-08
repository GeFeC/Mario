#pragma once

#include "Util/Util.hpp"
#include "config.hpp"

struct CollisionState{
  float distance_above = 0.f;
  float distance_below = 0.f;
  float distance_left = 0.f;
  float distance_right = 0.f;
};

struct CollisionRect{
  glm::vec2 position;
  glm::vec2 size;

  CollisionRect() = default;

  template<typename T>
  CollisionRect(const T& t){
    position = t.position;
    size = t.size;
  }

  CollisionRect(const glm::vec2& pos, const glm::vec2& size)
  : position(pos), size(size) {}
};


static auto CollisionPadding = 10.f;

static auto collision_intersects_in_x(const CollisionRect& object1, const CollisionRect& object2){
  return object1.position.x - object2.position.x 
    == util::in_range(-object1.size.x + CollisionPadding, object2.size.x - CollisionPadding);
}

static auto collision_intersects_in_y(const CollisionRect& object1, const CollisionRect& object2){
  return object1.position.y - object2.position.y 
    == util::in_range(-object1.size.y + CollisionPadding, object2.size.y - CollisionPadding);
}

static auto collision_intersects(const CollisionRect& object1, const CollisionRect& object2) {
  return 
    collision_intersects_in_x(object1, object2) &&
    collision_intersects_in_y(object1, object2);
}

static auto collision_controller(const CollisionRect& object1, const CollisionRect& object2){
  using util::BigValue;
  auto collision_state = CollisionState{ BigValue, BigValue, BigValue, BigValue };

  if (collision_intersects_in_x(object1, object2)){
    collision_state.distance_below 
      = -(object1.position.y + object1.size.y - object2.position.y);

    collision_state.distance_above 
      = std::abs(object2.position.y + object2.size.y - object1.position.y - CollisionPadding);
  }

  if (collision_intersects_in_y(object1, object2)){
    collision_state.distance_left 
      = (object1.position.x - (object2.size.x + object2.position.x - CollisionPadding + 0.1));

    collision_state.distance_right 
      = (object2.position.x - (object1.size.x + object1.position.x - CollisionPadding + 0.1));
  }

  return collision_state;
}

