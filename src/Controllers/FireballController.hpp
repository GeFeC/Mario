#pragma once

#include "Controllers/EntityController.hpp"
#include "States/EntityState.hpp"
#include "States/FireballState.hpp"
#include "States/LevelState.hpp"

namespace mario::fireball_controller{

static auto reset(FireballState& fireball){
  fireball.is_active = false;
  fireball.is_visible = false;
  fireball.gravity = 0.f;
  fireball.is_on_ground = false;

  fireball.explosion.start(fireball.position);
  fireball.position = glm::vec2(-util::BigValue);
}

static auto controller(FireballState& fireball, const LevelState& level){
  if (fireball.is_on_ground){
    fireball.gravity = FireballState::BouncePower;
    fireball.is_on_ground = false;
  }

  entity_controller::movement(fireball, level);
  entity_controller::gravity(fireball, level);
  fireball.explosion.run();

  if (fireball.is_active && fireball.acceleration.left == 0 && fireball.acceleration.right == 0){
    reset(fireball);
  }

  if (fireball.position.y > level.camera_offset.y + config::FrameBufferSize.y){
    reset(fireball);
  }

  const auto& player = level.player;
  if (fireball.is_active && std::abs(player.position.x - fireball.position.x) > config::FrameBufferSize.x){
    reset(fireball);
  }

  fireball.current_texture = &textures::fireball[level.fireball_counter.int_value()];
}

} //namespace mario::fireball_controller
