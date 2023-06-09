#pragma once

#include "Renderer/Texture.hpp"
#include "Renderer/Drawable.hpp"
#include "res/textures.hpp"
#include "config.hpp"

#include <glm/glm.hpp>
#include <unordered_map>

namespace mario{

struct EntityState{
  using Direction = int;
  using Flip = renderer::Drawable::Flip;

  inline static constexpr auto GravityForce = 70.f;
  inline static constexpr auto MovementSpeedMultiplier = 100.f;

  inline static constexpr auto DirectionLeft = -1;
  inline static constexpr auto DirectionRight = 1;

  struct Acceleration{
    float left = 0.f, right = 0.f;
  } acceleration;
  
  renderer::Texture const* current_texture = nullptr;
  Direction direction = DirectionRight;
  glm::vec2 position = { 0.f, 0.f };
  glm::vec2 size = { 0.f, 0.f };

  int vertical_flip = Flip::NoFlip;
  int texture_flip = Flip::NoFlip;
  
  float gravity = 0.f;
  float gravity_boost = 1.f;
  float death_delay = 1.f;

  //If monster was hit by a shell or fireball:
  bool was_hit = false;

  bool is_visible = true;
  bool is_active = false;
  bool is_dead = false;

  bool can_be_stomped = true;
  bool is_on_ground = false;
  bool is_on_platform = false;
  bool should_collide = true;
  bool fall_from_edge = true;

  auto set_direction(Direction direction, int speed){
    this->direction = direction;

    if (direction == DirectionRight){
      acceleration.right = speed;
      acceleration.left = 0.f;
      return;
    }

    acceleration.left = speed;
    acceleration.right = 0.f;
  }

  auto turn_around(){
    acceleration.left = acceleration.right = 0.f;
  }
};

} //namespace mario
