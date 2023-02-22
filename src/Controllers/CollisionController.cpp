#include "CollisionController.hpp"
#include "Util.hpp"
#include "config.hpp"

using util::Rect;

auto collision_controller(const Rect& object1, const Rect& object2) -> CollisionState{
  using config::BigValue;
  auto collision_state = CollisionState{ BigValue, BigValue, BigValue, BigValue };

  if (collision::is_hovering_in_x(object1, object2)){
    collision_state.distance_below 
      = -(object1.position.y + object1.size.y - object2.position.y);

    collision_state.distance_above 
      = std::abs(object2.position.y + object2.size.y - object1.position.y - CollisionOffset);
  }

  if (collision::is_hovering_in_y(object1, object2)){
    collision_state.distance_left 
      = (object1.position.x - (object2.size.x + object2.position.x - CollisionOffset + 0.1));

    collision_state.distance_right 
      = (object2.position.x - (object1.size.x + object1.position.x - CollisionOffset + 0.1));
  }

  return collision_state;
}

auto collision::is_hovering_in_x(const Rect& object1, const Rect& object2) -> bool{
  return object1.position.x - object2.position.x 
    == util::in_range(-object1.size.x + CollisionOffset, object2.size.x - CollisionOffset);
}

auto collision::is_hovering_in_y(const Rect& object1, const Rect& object2) -> bool{
  return object1.position.y - object2.position.y 
    == util::in_range(-object1.size.y + CollisionOffset, object2.size.y - CollisionOffset);
}

auto collision::is_hovering(const Rect& object1, const Rect& object2) -> bool{
  return is_hovering_in_x(object1, object2) && is_hovering_in_y(object1, object2);
}
