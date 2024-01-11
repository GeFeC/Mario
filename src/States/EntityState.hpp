#pragma once

#include "Util/Direction.hpp"
#include "Renderer/Texture.hpp"
#include "Util/Generator.hpp"

#include <glm/glm.hpp>

namespace mario{

struct EntityCopy{
	glm::vec2 position;
	float opacity = 0.f;
	bool is_active = false;
};

struct EntityState{
  static constexpr auto GravityForce = 70.f;
  static constexpr auto MovementSpeedMultiplier = 100.f;

  struct Acceleration{
    float left = 0.f, right = 0.f;
  } acceleration;

	util::Generator<EntityCopy> copies_generator;

  renderer::Texture const* current_texture = nullptr;
  util::Direction direction = util::Direction::right();
  util::Flip gravity_flip = util::Flip::no_flip();

  glm::vec2 position = { 0.f, 0.f };
  glm::vec2 size = { 0.f, 0.f };

  util::Flip vertical_flip = util::Flip::no_flip();
  util::Flip texture_flip = util::Flip::no_flip();
  
  float gravity = 0.f;
  float gravity_boost = 1.f;
  float death_delay = 1.f;

  //If monster was hit by a shell or fireball:
  bool was_hit = false;

	bool follows_player = false;
  bool is_visible = true;
  bool is_active = false;
  bool is_dead = false;

	bool should_create_copy = true;
  bool can_be_stomped = true;
  bool is_on_ground = false;
  bool is_on_platform = false;
  bool should_collide = true;
  bool fall_from_edge = true;

  auto flip_gravity(){
    gravity_flip.toggle();
    vertical_flip.toggle();
  }

  auto set_direction(util::Direction direction, int speed){
    this->direction = direction;

    if (direction.is_right()){
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

  auto total_speed(){
    return std::abs(acceleration.right - acceleration.left);
  }
};

} //namespace mario
