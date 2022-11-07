#pragma once

#include "Renderer/Texture.hpp"
#include "res/textures.hpp"

#include <glm/glm.hpp>

struct EntityState{
  static constexpr auto DirectionLeft = -1;
  static constexpr auto DirectionRight = 1;
  using Direction = int;

  struct Acceleration{
    float left, right;
  } acceleration;
  
  Texture* current_texture = &textures::small_mario;
  Direction direction = DirectionRight;
  glm::vec2 position = { 60.f, 0.f };
  glm::vec2 size = { 60.f, 60.f };
  bool is_visible = true;
  
  float gravity = 0.f;
  float death_delay = 1.f;

  int points_index = -1;

  int mobs_killed_in_row = 1;

  bool is_on_ground = false;
  bool is_dead = false;
  bool is_active = false;

};
