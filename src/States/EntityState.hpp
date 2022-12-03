#pragma once

#include "Renderer/Texture.hpp"
#include "Renderer/Drawable.hpp"
#include "res/textures.hpp"

#include <glm/glm.hpp>

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
      return;
    }

    acceleration.left = speed;
  }
};

struct MushroomState : EntityState{
  float offset = 0.f;
  bool should_be_pushed_out = false;
};

struct PlayerState : EntityState{
  enum class Growth{
    Small,
    Medium,
    Big
  } growth = Growth::Small;

  int mobs_killed_in_row = 1;
  float grow_state = 0.f;
  float invincibility_delay = 0.f;
  bool is_squating = false;
  bool must_squat = false;
  bool is_growing_up = false;
  bool is_shrinking = false;

  auto default_texture() const{
    switch(growth){
      case Growth::Small: return &textures::small_mario;
      case Growth::Medium: return &textures::medium_mario;
      case Growth::Big: return &textures::big_mario;
      default: return &textures::small_mario;
    }
  }

  auto walk_texture(int walk_frame) const{
    switch(growth){
      case Growth::Small: return &textures::small_mario_walk[walk_frame];
      case Growth::Big: return &textures::big_mario_walk[walk_frame];
      default: return &textures::small_mario;
    }
  }

  auto turn_texture() const{
    switch(growth){
      case Growth::Small: return &textures::small_mario_turning;
      case Growth::Big: return &textures::big_mario_turning;
      default: return &textures::small_mario;
    }
  }

  auto jump_texture() const{
    switch(growth){
      case Growth::Small: return &textures::small_mario_jumping;
      case Growth::Big: return &textures::big_mario_jumping;
      default: return &textures::small_mario;
    }
  }
};
