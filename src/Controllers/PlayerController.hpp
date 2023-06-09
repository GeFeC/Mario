#pragma once

#include "Controllers/CollisionController.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/FireballController.hpp"
#include "States/EntityState.hpp"

#include "config.hpp"
#include "res/textures.hpp"
#include "Window.hpp"
#include "Util/Util.hpp"

#include <GLFW/glfw3.h>
#include <cmath>
#include <algorithm>

namespace mario::player_controller{

static auto detect_collision_above(PlayerState& player, LevelState& level){
  entity_controller::detect_collision_with_level(player, level, [&](const auto& collision_state){
    if (collision_state.distance_above < -player.gravity){
      player.gravity = -collision_state.distance_above;
    }

    if (collision_state.distance_below < BlockBase::Size && !window::is_key_pressed(GLFW_KEY_UP)){
      player.jump_cooldown = false;
    }
  });
}

static auto swim(PlayerState& player, LevelState& level){
  player.swim_counter.run();

  if (player.is_dead) return;
  player.gravity = std::min(player.gravity, PlayerState::MaxGravityWhenSwimming);

  player.gravity_boost = 0.05f;

  [&]{
    if (player.position.y < BlockBase::Size * 1.5f) return;

    if (window::is_key_pressed(GLFW_KEY_UP) && !player.swim_cooldown && !player.is_squating){
      player.swim_counter.reset();
      player.gravity = PlayerState::SwimPower;
      player.is_on_ground = false;
      player.swim_cooldown = true;
    }
  }();

  if (!window::is_key_pressed(GLFW_KEY_UP)){
    player.swim_cooldown = false;
  }

  detect_collision_above(player, level);
}

static auto jump(PlayerState& player, LevelState& level){
  if (player.is_dead) return;

  if (window::is_key_pressed(GLFW_KEY_UP) && player.is_on_ground && !player.jump_cooldown){
    player.gravity = PlayerState::JumpPower;
    player.is_on_ground = false;
    player.jump_cooldown = true;
  }

  detect_collision_above(player, level);
}

static auto gravity(PlayerState& player, LevelState& level){
  player.gravity_boost = 1.f;
  if (!player.is_dead && player.gravity < 0 && !window::is_key_pressed(GLFW_KEY_UP)){
    player.gravity_boost = 2.f;
  }

  entity_controller::gravity(player, level);
}

static auto movement(PlayerState& player, LevelState& level){
  if (!player.can_move) return;

  const auto Right = EntityState::DirectionRight;
  const auto Left = EntityState::DirectionLeft;

  const auto speed_boost = window::delta_time * 12;

  //Keyboard events:
  if (window::is_key_pressed(GLFW_KEY_RIGHT)){
    player.acceleration.right += speed_boost;
    player.direction = Right;
  }
  else{ 
    player.acceleration.right -= speed_boost / player.slip;
  }

  if (window::is_key_pressed(GLFW_KEY_LEFT)){
    player.acceleration.left += speed_boost;
    player.direction = Left;
  }
  else{
    player.acceleration.left -= speed_boost / player.slip;
  }

  //Calculating max speed
  if (!player.is_squating){
    if (window::is_key_pressed(GLFW_KEY_LEFT_CONTROL)){
      player.max_speed = PlayerState::MaxSpeedWithSprint;
    }
    else if (player.max_speed > PlayerState::MaxSpeedWithoutSprint){
      player.max_speed -= speed_boost;
    }
    else{
      player.max_speed = PlayerState::MaxSpeedWithoutSprint;
    }
  }

  if (player.is_squating && player.max_speed > PlayerState::MaxSpeedWhenSquating){
    player.max_speed -= speed_boost;
  }

  player.acceleration.left = std::clamp(player.acceleration.left, 0.f, player.max_speed);
  player.acceleration.right = std::clamp(player.acceleration.right, 0.f, player.max_speed);
}

static auto grow(PlayerState& player){
  player.growth_counter.run();
  player.grow_state = player.growth_counter.int_value();

  if(player.growth_counter.stopped_counting()){
    player.is_growing_up = false;
    player.growth_counter.reset();
  }
}

static auto shrink(PlayerState& player){
  player.form = PlayerState::Form::Normal;
  player.growth_counter.run();
  player.grow_state = 2 - player.growth_counter.int_value();

  if (player.growth_counter.stopped_counting()){
    player.is_shrinking = false;
    player.is_squating = false;
    player.growth_counter.reset();

    player.invincibility_delay = 2.f; //2 Seconds
  }
}

static auto invincibility(PlayerState& player){
  if (player.invincibility_delay <= 0.f) {
    player.is_visible = true;
    return;
  }

  const auto invincibility = player.invincibility_delay * 100 | util::as<int>;
  if (invincibility % 3 == 0){
    player.is_visible = !player.is_visible;
  }

  player.invincibility_delay -= window::delta_time;
}

static auto update_growth(PlayerState& player){
  if (!player.is_growing_up && !player.is_shrinking) return;

  const auto grow_state = player.grow_state | util::as<int>;

  const auto set_grow_state = [&](auto growth, auto new_player_size){
    player.growth = growth;

    if (player.size.y != new_player_size){
      player.position.y += player.size.y - new_player_size;
      player.size.y = new_player_size;
    }
  };

  if (grow_state % 3 == 0){
    set_grow_state(PlayerState::Growth::Small, BlockBase::Size);
  }

  if (grow_state % 3 == 1){
    set_grow_state(PlayerState::Growth::Medium, BlockBase::Size * 1.5f);
  }

  if (grow_state % 3 == 2){
    set_grow_state(PlayerState::Growth::Big, BlockBase::Size * 2.f);
  }
}

static auto transform_to_fire(PlayerState& player){
  player.transformation_counter.run();
  const auto transformation_state = player.transformation_counter.int_value();

  if (transformation_state == 1) player.form = PlayerState::Form::Black;
  else if (transformation_state == 0) player.form = PlayerState::Form::Fire;

  if (player.transformation_counter.stopped_counting()){
    player.transformation_counter.reset();
    player.is_changing_to_fire = false;
  }
}

static auto textures(PlayerState& player, const LevelState& level){
  player.current_texture = player.default_texture();

  if (player.is_growing_up || player.is_shrinking) return;

  //Walking
  auto &current_walk_animation_frame = player.current_walk_animation_frame;
  if (player.acceleration.left > 0 || player.acceleration.right > 0){
    const auto total_speed = std::abs(player.acceleration.left - player.acceleration.right);

    current_walk_animation_frame += window::delta_time * total_speed * 3.f;
    if (current_walk_animation_frame >= 3.f){
      current_walk_animation_frame = 0.f;
    }
    
    player.current_texture = player.walk_texture(current_walk_animation_frame);
  }
  else{
    current_walk_animation_frame = 0.f;
  }

  const auto total_speed = player.acceleration.right - player.acceleration.left;
  const auto is_turning_left = total_speed > 0 && player.direction == EntityState::DirectionLeft;
  const auto is_turning_right = total_speed < 0 && player.direction == EntityState::DirectionRight;

  if (is_turning_left || is_turning_right){
    player.current_texture = player.turn_texture();
  }

  if (!player.is_on_ground){
    player.current_texture = player.jump_texture();
  }

  //Swimming
  if (level.biome == LevelState::Biome::Underwater && !player.is_on_ground){
    player.current_texture = player.swim_texture(player.swim_counter.int_value());
  }

  if (player.is_dead){
    player.current_texture = &textures::small_mario_dead;
  }

  if (player.is_squating){
    player.current_texture = player.squat_texture();
  }
}

static auto death(PlayerState& player){
  if (player.is_dead && player.death_delay > 0.f){
    player.death_delay -= window::delta_time;
  }

  static constexpr auto VerySmallValue = -1000.f;

  if (player.death_delay == util::in_range(VerySmallValue, 0.f)){
    player.gravity = PlayerState::DeathBouncePower;
    player.death_delay = VerySmallValue - 1;
  }
}

static auto squat(PlayerState& player, LevelState& level){
  if (player.is_on_ground){
    player.mobs_killed_in_row = 1;
  }

  auto is_forced_to_squat = false;
  entity_controller::detect_collision_with_level(player, level, [&](const auto& collision_state){
    if (collision_state.distance_above == util::in_range(0, 20) && player.is_squating){
      is_forced_to_squat = true;
    }
  });

  if (player.growth == PlayerState::Growth::Big){
    if (window::is_key_pressed(GLFW_KEY_DOWN) || is_forced_to_squat){
      if (player.size.y == BlockBase::Size * 2){
        player.is_squating = true;
        player.position.y += BlockBase::Size;
      }

      player.size.y = BlockBase::Size;
    }
    else{
      if (player.size.y == BlockBase::Size){
        player.is_squating = false;
        player.position.y -= BlockBase::Size;
      }

      player.size.y = BlockBase::Size * 2;
    }
  }
}

static auto fireballs(PlayerState& player, const LevelState& level){
  for (auto& fireball : player.fireballs){
    fireball_controller::controller(fireball, level); 
  }

  if (player.form != PlayerState::Form::Fire) return;

  static auto key_active = true;
  const auto shoot_key_pressed = window::is_key_pressed(GLFW_KEY_LEFT_ALT);

  if (!shoot_key_pressed) key_active = true;
  if (!shoot_key_pressed || !key_active) return;
  key_active = false;

  const auto fireball_ptr = std::find_if(player.fireballs.begin(), player.fireballs.end(), [](const auto& f){
    return !f.is_active && f.explosion.finished();
  });

  if (fireball_ptr == player.fireballs.end()) return;

  auto& fireball = *fireball_ptr;
  fireball.position = player.position + player.size / 2.f - fireball.size / 2.f;
  fireball.set_direction(player.direction, PlayerState::FireballSpeed);
  fireball.is_active = true;
  fireball.is_visible = true;
}

static auto controller(PlayerState& player, LevelState& level) -> void{
  if (player.is_changing_to_fire){
    transform_to_fire(player);
  }
  else if (player.is_growing_up){
    grow(player);
  }
  else if (player.is_shrinking){
    shrink(player);
  }
  else{
    movement(player, level);
    entity_controller::movement(player, level);

    if (level.biome == LevelState::Biome::Underwater && player.position.y < config::FrameBufferSize.y){
      swim(player, level);
    }
    else{
      jump(player, level);
    }

    gravity(player, level);
    squat(player, level);
    invincibility(player);
    fireballs(player, level);

    death(player);
  } 

  update_growth(player);
  textures(player, level);
}

static auto can_hit_block_above(const PlayerState& player, const BouncingBlockState& block) -> bool{
  using collision_controller::CollisionPadding;

  if (player.direction == EntityState::DirectionLeft){
    return player.position.x - block.position.x 
      == util::in_range(-CollisionPadding, block.size.x - CollisionPadding);
  }

  return player.position.x - block.position.x 
    == util::in_range(-player.size.x + CollisionPadding, CollisionPadding);
}

static auto did_hit_block_above(const PlayerState& player, const BouncingBlockState& block) -> bool{
  if (player.is_dead) return false;

  const auto was_block_hit_by_player 
    = (player.position.y - block.position.y - block.size.y) == util::in_range(-10, -5);

  return 
    can_hit_block_above(player, block) && 
    was_block_hit_by_player && 
    !block.bounce_state.is_bouncing;
}

} //namespace mario::player_controller