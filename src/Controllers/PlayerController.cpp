#include "PlayerController.hpp"

#include "Controllers/CollisionController.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/GoombaController.hpp"
#include "Controllers/FireballController.hpp"
#include "States/EntityState.hpp"

#include "config.hpp"
#include "res/textures.hpp"
#include "Window.hpp"
#include "Util.hpp"

#include <GLFW/glfw3.h>
#include <cmath>
#include <algorithm>

static auto to_grid_coords(const glm::vec2& normal_coords){
  return glm::ivec2(normal_coords.x / 60, normal_coords.y / 60);
}

static auto player_jump(EntityState& player, LevelState& level){
  if (player.is_dead) return;

  if (window::is_key_pressed(GLFW_KEY_UP) && player.is_on_ground){
    player.gravity = -22;
    player.is_on_ground = false;
  }

  detect_entity_collision_with_level(player, level, [&](const auto& collision_state){
    if (collision_state.distance_above < -player.gravity){
      player.gravity = -collision_state.distance_above;
    }
  });
}

static auto player_gravity(EntityState& player, LevelState& level){
  int gravity_boost = 1;
  if (!player.is_dead && player.gravity < 0 && !window::is_key_pressed(GLFW_KEY_UP)){
    gravity_boost = 2;
  }

  entity_gravity(player, level, gravity_boost);
}

static auto player_movement(PlayerState& player, LevelState& level){
  const auto Right = EntityState::DirectionRight;
  const auto Left = EntityState::DirectionLeft;

  const auto speed_boost = window::delta_time * 8;

  //Keyboard events:
  if (window::is_key_pressed(GLFW_KEY_RIGHT)){
    player.acceleration.right += speed_boost;
    player.direction = Right;
  }
  else{
    player.acceleration.right -= speed_boost;
  }

  if (window::is_key_pressed(GLFW_KEY_LEFT)){
    player.acceleration.left += speed_boost;
    player.direction = Left;
  }
  else{
    player.acceleration.left -= speed_boost;
  }

  //Calculating max speed
  static auto max_speed = 5.f;

  if (!player.is_squating){
    if (window::is_key_pressed(GLFW_KEY_LEFT_CONTROL)){
      max_speed = 7.f;
    }
    else if (max_speed > 5.f){
      max_speed -= speed_boost;
    }
    else{
      max_speed = 5.f;
    }
  }

  if (player.is_squating && max_speed > 1.f){
    max_speed -= speed_boost;
  }

  player.acceleration.left = std::clamp(player.acceleration.left, 0.f, max_speed);
  player.acceleration.right = std::clamp(player.acceleration.right, 0.f, max_speed);
}

static auto player_grow_up(PlayerState& player){
  player.growth_counter.run();
  player.grow_state = player.growth_counter.as_int();

  if(player.growth_counter.stopped_counting()){
    player.is_growing_up = false;
    player.growth_counter.reset();
  }
}

static auto player_shrink_down(PlayerState& player){
  player.form = PlayerState::Form::Normal;
  player.growth_counter.run();
  player.grow_state = 2 - player.growth_counter.as_int();

  if (player.growth_counter.stopped_counting()){
    player.is_shrinking = false;
    player.is_squating = false;
    player.growth_counter.reset();

    player.invincibility_delay = 2.f; //2 Seconds
  }
}

static auto player_invincibility(PlayerState& player){
  if (player.invincibility_delay <= 0.f) {
    player.is_visible = true;
    return;
  }

  const auto invincibility = static_cast<int>(player.invincibility_delay * 100);
  if (invincibility % 3 == 0){
    player.is_visible = !player.is_visible;
  }

  player.invincibility_delay -= window::delta_time;
}

static auto player_update_growth(PlayerState& player){
  if (!player.is_growing_up && !player.is_shrinking) return;

  const auto grow_state = static_cast<int>(player.grow_state);

  const auto set_grow_state = [&](auto growth, auto new_player_size){
    player.growth = growth;

    if (player.size.y != new_player_size){
      player.position.y += player.size.y - new_player_size;
      player.size.y = new_player_size;
    }
  };

  if (grow_state % 3 == 0){
    set_grow_state(PlayerState::Growth::Small, config::BlockSize);
  }

  if (grow_state % 3 == 1){
    set_grow_state(PlayerState::Growth::Medium, config::BlockSize * 1.5f);
  }

  if (grow_state % 3 == 2){
    set_grow_state(PlayerState::Growth::Big, config::BlockSize * 2.f);
  }
}

auto player_transform_to_fire(PlayerState& player){
  player.transformation_counter.run();
  const auto transformation_state = player.transformation_counter.as_int();

  if (transformation_state == 1) player.form = PlayerState::Form::Black;
  else if (transformation_state == 0) player.form = PlayerState::Form::Fire;

  if (player.transformation_counter.stopped_counting()){
    player.transformation_counter.reset();
    player.is_changing_to_fire = false;
  }
}

static auto player_textures(PlayerState& player){
  player.current_texture = player.default_texture();

  if (player.is_growing_up || player.is_shrinking) return;

  static auto current_walk_animation_frame = 0.f;
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

  if (player.is_dead){
    player.current_texture = &textures::small_mario_dead;
  }

  if (player.is_squating){
    player.current_texture = player.squat_texture();
  }
}

static auto player_death(EntityState& player){
  if (player.is_dead && player.death_delay > 0.f){
    player.death_delay -= window::delta_time;
  }

  static constexpr auto VerySmallValue = -1000.f;

  if (player.death_delay | util::in_range(VerySmallValue, 0.f)){
    player.gravity = -15;
    player.death_delay = VerySmallValue - 1;
  }
}

auto player_squat(PlayerState& player, LevelState& level){
  if (player.is_on_ground){
    player.mobs_killed_in_row = 1;
  }

  auto is_forced_to_squat = false;
  detect_entity_collision_with_level(player, level, [&](const auto& collision_state){
    if (collision_state.distance_above | util::in_range(0, 20) && player.is_squating){
      is_forced_to_squat = true;
    }
  });

  if (player.growth == PlayerState::Growth::Big){
    if (window::is_key_pressed(GLFW_KEY_DOWN) || is_forced_to_squat){
      if (player.size.y == config::BlockSize * 2){
        player.is_squating = true;
        player.position.y += config::BlockSize;
      }

      player.size.y = config::BlockSize;
    }
    else{
      if (player.size.y == config::BlockSize){
        player.is_squating = false;
        player.position.y -= config::BlockSize;
      }

      player.size.y = config::BlockSize * 2;
    }
  }
}

static auto player_fireballs(PlayerState& player, const LevelState& level){
  for (auto& fireball : player.fireballs){
    fireball_controller(fireball, level); 
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
  fireball.set_direction(player.direction, 12.f);
  fireball.is_active = true;
  fireball.is_visible = true;
}

auto player_controller(PlayerState& player, LevelState& level) -> void{
  if (player.is_changing_to_fire){
    player_transform_to_fire(player);
  }
  else if (player.is_growing_up){
    player_grow_up(player);
  }
  else if (player.is_shrinking){
    player_shrink_down(player);
  }
  else{
    player_movement(player, level);
    entity_movement(player, level);
    player_jump(player, level);
    player_gravity(player, level);
    player_squat(player, level);
    player_invincibility(player);
    player_fireballs(player, level);

    player_death(player);
  } 

  player_update_growth(player);
  player_textures(player);
}

auto player_stomp_on_entity(const EntityState& player, const EntityState& entity) -> bool{
  if (collision::is_hovering_in_x(player, entity) && !entity.is_dead && player.gravity > 0){
    return entity.position.y - player.position.y - player.size.y | util::in_range(-30, 0);
  }

  return false;
}
