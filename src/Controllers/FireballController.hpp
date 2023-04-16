#pragma once

#include "Controllers/EntityController.hpp"
#include "States/EntityState.hpp"
#include "States/FireballState.hpp"
#include "States/LevelState.hpp"

static auto fireball_controller(FireballState& fireball, const LevelState& level){
  if (fireball.is_on_ground){
    fireball.gravity = FireballState::BouncePower;
    fireball.is_on_ground = false;
  }

  entity_movement(fireball, level);
  entity_gravity(fireball, level);
  fireball.explosion.run();

  const auto reset_fireball = [](FireballState& fireball){
    fireball.is_active = false;
    fireball.is_visible = false;
    fireball.gravity = 0.f;

    fireball.explosion.start(fireball.position);
    fireball.position = glm::vec2(0);
  };

  if (fireball.is_active && fireball.acceleration.left == 0 && fireball.acceleration.right == 0){
    reset_fireball(fireball);
  }

  if (fireball.position.y > level.camera_offset.y + config::FrameBufferSize.y){
    reset_fireball(fireball);
  }

  const auto& player = level.player;
  if (fireball.is_active && std::abs(player.position.x - fireball.position.x) > config::FrameBufferSize.x){
    reset_fireball(fireball);
  }

  fireball.current_texture = &textures::fireball[level.fireball_counter.int_value()];
}
