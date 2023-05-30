#pragma once

#include "Controllers/CollisionController.hpp"
#include "Renderer/Drawable.hpp"
#include "States/BlockState.hpp"
#include "States/EntityPusherState.hpp"
#include "States/EntityState.hpp"
#include "States/LevelState.hpp"
#include "States/MonsterState.hpp"
#include "Util/Util.hpp"
#include "Window.hpp"
#include "config.hpp"

static auto detect_entity_collision_with_level = [](EntityState& entity, const LevelState& level, auto callable){
  if (!entity.should_collide){
    return;
  }

  //Blocks
  for (const auto& block : level.game_objects.get_vec<BlockState>()){
    if (!block.is_solid) continue;

    const auto collision_state = collision_controller(util::Rect(entity), util::Rect(block));
    callable(collision_state);
  }

  //Platforms
  level.game_objects.for_each_type<PlatformState, LoopedPlatformState>([&](const auto& platform){
    const auto platform_rect = util::Rect(
      platform.position,
      platform.size()
    );

    const auto collision_state = collision_controller(util::Rect(entity), platform_rect);
    callable(collision_state);
  });

  if (entity.fall_from_edge) return;
  if (!entity.is_on_ground) return;
  if (entity.is_on_platform) return;

  //Checking if entity should change direction not to fall from edge
  static constexpr auto Offset = MonsterState::EdgeDetectionOffset;

  const auto x = (entity.position.x) / BlockBase::Size;
  const auto y = (entity.position.y + entity.size.y + Offset) / BlockBase::Size;
  const auto left_x = (entity.position.x + Offset) / BlockBase::Size;
  const auto right_x = (entity.position.x + BlockBase::Size - Offset) / BlockBase::Size;

  const auto level_size = level.max_size();

  if (left_x >= level_size.x || left_x < 0) return;
  if (right_x >= level_size.x || right_x < 0) return;
  if (y >= level_size.y || y - 1 <= 0) return;

  static constexpr auto Air = 0;
  if (level.hitbox_grid[right_x][y] == Air && entity.direction == EntityState::DirectionRight){
    entity.acceleration.left = entity.acceleration.right = 0.f;
    entity.position.x = (right_x - 1) * BlockBase::Size + Offset;
  }

  if (level.hitbox_grid[left_x][y] == Air && entity.direction == EntityState::DirectionLeft){
    entity.acceleration.left = entity.acceleration.right = 0.f;
    entity.position.x = left_x * BlockBase::Size - Offset;
  }
};

static auto entity_movement(EntityState& entity, const LevelState& level){
  if (entity.is_dead || !entity.is_active) return;

  auto left_boost = window::delta_time * entity.acceleration.left * EntityState::MovementSpeedMultiplier;
  auto right_boost = window::delta_time * entity.acceleration.right * EntityState::MovementSpeedMultiplier;

  detect_entity_collision_with_level(entity, level, [&](const auto& collision_state){
    const auto distance_left = std::abs(collision_state.distance_left);
    const auto distance_right = std::abs(collision_state.distance_right);

    if (collision_state.distance_right == util::in_range(-1.f, 0.f)){
      entity.position.x += collision_state.distance_right;
    }

    if (collision_state.distance_left == util::in_range(-1.f, 0.f)){
      entity.position.x -= collision_state.distance_left;
    }

    if (distance_left < left_boost){
      left_boost = distance_left;
      entity.acceleration.left = 0.f;
    }
    if (distance_right < right_boost){
      right_boost = distance_right;
      entity.acceleration.right = 0.f;
    }
  });

  entity.position.x += right_boost;
  entity.position.x -= left_boost;
}

template<typename Callable>
static auto entity_gravity_base(EntityState& entity, const LevelState& level, const Callable& collision_callback){
  if (entity.is_dead && entity.death_delay > 0.f) return;
  if (!entity.is_active) return;

  if (entity.is_on_ground && !entity.is_dead && entity.should_collide) entity.gravity = 0.f;
  entity.is_on_ground = false;

  entity.gravity += window::delta_time * EntityState::GravityForce * entity.gravity_boost;

  auto position_increaser = entity.gravity * window::delta_time * 70.f;

  detect_entity_collision_with_level(entity, level, [&](const auto& collision_state){
    collision_callback(collision_state, position_increaser);
  });

  static constexpr auto MaxGravityForce = 100.f;

  position_increaser = std::min(position_increaser, MaxGravityForce);
  entity.position.y += position_increaser;
}

static auto entity_gravity(EntityState& entity, const LevelState& level){
  entity_gravity_base(entity, level, [&](const auto& collision_state, auto& position_increaser){
    if (entity.death_delay <= 0.f) return;

    if (collision_state.distance_above < -position_increaser){
      position_increaser = -collision_state.distance_above + 1.f;
      entity.gravity = 0.f;
    }

    if (collision_state.distance_below == util::in_range(-CollisionOffset, position_increaser)){
      entity.is_on_ground = true;
      position_increaser = collision_state.distance_below;
    }
  });
}

static auto player_is_on_entity(const PlayerState& player, const EntityState& entity) -> bool{
  if (!entity.can_be_stomped) return false;

  if (collision::is_hovering_in_x(player, entity)){
    const auto distance = entity.position.y - player.position.y - player.size.y;
    const auto hitbox_tolerance = entity.can_be_stomped
      ? -entity.size.y + BlockBase::Size / 6.f
      : -BlockBase::Size / 6.f;

    return distance == util::in_range(hitbox_tolerance, 0);
  }

  return false;
}

static auto entity_kill_player_on_touch(const EntityState& entity, PlayerState& player){
  if (!entity.is_active) return;
  if (entity.is_dead) return;
  if (player.is_dead) return;
  if (player_is_on_entity(player, entity)) return;
  if (!collision::is_hovering(player, entity)) return;

  if (player.growth == PlayerState::Growth::Big){
    player.is_shrinking = true;
  }
  else if (!player.is_shrinking && player.invincibility_delay <= 0.f) { 
    player.is_dead = true; 
  }
};

static auto entity_was_stomped(const PlayerState& player, const EntityState& entity) -> bool{
  if (!entity.can_be_stomped) return false;
  if (!entity.should_collide) return false;
  if (entity.is_dead) return false;
  if (player.is_dead) return false;
  if (player.gravity < 0) return false;

  return player_is_on_entity(player, entity);
}
