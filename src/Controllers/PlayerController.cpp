#include "PlayerController.hpp"
#include "Controllers/CollisionController.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/GoombaController.hpp"
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

static auto player_movement(EntityState& player, LevelState& level){
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

  if (window::is_key_pressed(GLFW_KEY_LEFT_CONTROL)){
    max_speed = 7.f;
  }
  else if (max_speed > 5.f){
    max_speed -= speed_boost;
  }
  else{
    max_speed = 5.f;
  }

  player.acceleration.left = std::clamp(player.acceleration.left, 0.f, max_speed);
  player.acceleration.right = std::clamp(player.acceleration.right, 0.f, max_speed);

}

static auto player_textures(EntityState& player){
  player.current_texture = &textures::small_mario;

  static auto current_walk_animation_frame = 0.f;
  if (player.acceleration.left > 0 || player.acceleration.right > 0){
    const auto total_speed = std::abs(player.acceleration.left - player.acceleration.right);

    current_walk_animation_frame += window::delta_time * total_speed * 3.f;
    if (current_walk_animation_frame >= 3.f){
      current_walk_animation_frame = 0.f;
    }
    
    player.current_texture = &textures::small_mario_walk[static_cast<int>(current_walk_animation_frame)];
  }
  else{
    current_walk_animation_frame = 0.f;
  }

  const auto total_speed = player.acceleration.right - player.acceleration.left;
  const auto is_turning_left = total_speed > 0 && player.direction == EntityState::DirectionLeft;
  const auto is_turning_right = total_speed < 0 && player.direction == EntityState::DirectionRight;

  if (is_turning_left || is_turning_right){
    player.current_texture = &textures::small_mario_turning;
  }

  if (!player.is_on_ground){
    player.current_texture = &textures::small_mario_jumping;
  }

  if (player.is_dead){
    player.current_texture = &textures::small_mario_dead;
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

auto player_controller(EntityState& player, LevelState& level) -> void{
  player_movement(player, level);
  player_jump(player, level);
  player_gravity(player, level);

  player_death(player);

  if (player.is_on_ground){
    player.mobs_killed_in_row = 1;
  }

  player_textures(player);
}

auto player_hit_block_above(const EntityState& player, const BlockState& block) -> bool{
  const auto was_block_hit_by_player = (player.position.y - block.position.y - block.size.y) | util::in_range(-10, 0);

  return 
    player_can_hit_block_above(player, block) && 
    was_block_hit_by_player && 
    !block.bounce_state.is_bouncing && 
    player.gravity > 0.f;
}

auto player_can_hit_block_above(const EntityState& player, const BlockState& block) -> bool{
  if (player.direction == EntityState::DirectionLeft){
    return player.position.x - block.position.x 
      | util::in_range(0.f, block.size.x - CollisionOffset);
  }

  return player.position.x - block.position.x 
    | util::in_range(-player.size.x + CollisionOffset, 0.f);
}

auto player_stomp_on_entity(const EntityState& player, const EntityState& entity) -> bool{
  if (collision::is_hovering_in_x(player, entity) && !entity.is_dead){
    return entity.position.y - player.position.y - player.size.x | util::in_range(0, 20);
  }

  return false;
}

