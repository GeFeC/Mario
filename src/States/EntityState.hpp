#pragma once

#include "Renderer/Texture.hpp"
#include "Renderer/Drawable.hpp"
#include "res/textures.hpp"
#include "config.hpp"

#include <glm/glm.hpp>
#include <unordered_map>

struct EntityState{
  using Direction = int;
  using Flip = Drawable::Flip;

  static constexpr auto DirectionLeft = -1;
  static constexpr auto DirectionRight = 1;

  struct Acceleration{
    float left = 0.f, right = 0.f;
  } acceleration;
  
  Texture const* current_texture = nullptr;
  Direction direction = DirectionRight;
  glm::vec2 position = { 60.f, 0.f };
  glm::vec2 size = { 60.f, 60.f };
  int vertical_flip = Flip::NoFlip;
  
  float gravity = 0.f;
  float death_delay = 1.f;

  int points_index = -1;

  bool is_visible = true;
  bool is_on_ground = false;
  bool is_dead = false;
  bool is_active = false;
  bool should_collide = true;

  auto set_direction(Direction direction, int speed){
    if (direction == DirectionRight){
      acceleration.right = speed;
      acceleration.left = 0.f;
      return;
    }

    acceleration.left = speed;
    acceleration.right = 0.f;
  }
};

struct MushroomState : EntityState{
  float offset = 0.f;
  bool should_be_pushed_out = false;
};

