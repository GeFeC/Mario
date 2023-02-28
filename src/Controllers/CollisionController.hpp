#pragma once

#include "States/CollisionState.hpp"
#include "config.hpp"
#include "Util/Util.hpp"

#include <algorithm>
#include <utility>
#include <glm/glm.hpp>

inline constexpr auto CollisionOffset = 10.f; 

auto collision_controller(const util::Rect& object1, const util::Rect& object2) -> CollisionState;

namespace collision{
  auto is_hovering_in_x(const util::Rect& object1, const util::Rect& object2) -> bool;
  auto is_hovering_in_y(const util::Rect& object1, const util::Rect& object2) -> bool;

  auto is_hovering(const util::Rect& object1, const util::Rect& object2) -> bool;
}
