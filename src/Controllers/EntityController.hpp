#pragma once

#include "Controllers/CollisionController.hpp"
#include "Renderer/Drawable.hpp"
#include "States/BlockState.hpp"
#include "States/EntityPusherState.hpp"
#include "States/EntityState.hpp"
#include "States/LevelState.hpp"
#include "States/MonsterState.hpp"
#include "Util/Loop.hpp"
#include "Util/Util.hpp"
#include "Window.hpp"
#include "config.hpp"

#include "Terminal.hpp"

namespace mario::entity_controller{

static auto detect_collision_with_level = [](EntityState& entity, const LevelState& level, auto callable){
  if (!entity.should_collide){
    return;
  }

  //Get blocks surrounding entity
  const auto min_index = std::make_pair(
    entity.position.x / BlockBase::Size - 1,
    std::max(entity.position.y / BlockBase::Size - 1, 0.f)
  );

  const auto max_index = std::make_pair(
    min_index.first + 1 + entity.size.x / BlockBase::Size + 1,
    std::min(min_index.second + 1 + entity.size.y / BlockBase::Size + 1, level.max_size().y)
  );

  auto blocks_surrounding_entity = std::vector<BlockState>();
  util::for_2d([&](auto x, auto y){
    const auto is_block_outside_the_world = x != util::in_range(0.f, level.max_size().x - 1);

    if (is_block_outside_the_world && !level.should_handle_hitbox_on_sides) return;

    if (is_block_outside_the_world || level.hitbox_grid[x][y] == LevelState::HitboxState::Solid){
      blocks_surrounding_entity.push_back(BlockState({ x, y }, &textures::dirt));
    }
  }, min_index, max_index);

  //Blocks
  for (const auto& block : blocks_surrounding_entity){
    if (!block.is_solid) continue;

    const auto collision_state = collision_controller::controller(
      collision_controller::Rect(entity), 
      collision_controller::Rect(block)
    );
    callable(collision_state);
  }

  //Platforms
  level.game_objects.for_each_type<PlatformState, LoopedPlatformState>([&](const auto& platform){
    const auto platform_rect = collision_controller::Rect(
      platform.position,
      platform.size()
    );

    const auto collision_state = collision_controller::controller(
      collision_controller::Rect(entity), 
      platform_rect
    );

    callable(collision_state);
  });

  if (entity.fall_from_edge) return;
  if (!entity.is_on_ground) return;
  if (entity.is_on_platform) return;

  //Checking if entity should change direction not to fall from edge
  static constexpr auto Offset = MonsterState::EdgeDetectionOffset;

  const auto y = (entity.position.y + entity.size.y + Offset) / BlockBase::Size;
  const auto left_x = (entity.position.x + Offset) / BlockBase::Size;
  const auto right_x = (entity.position.x + BlockBase::Size - Offset) / BlockBase::Size;

  const auto level_size = level.max_size();

  if (left_x >= level_size.x || left_x < 0) return;
  if (right_x >= level_size.x || right_x < 0) return;
  if (y >= level_size.y || y - 1 <= 0) return;

  using Hitbox = LevelState::HitboxState;
  if (level.hitbox_grid[right_x][y] == Hitbox::NonSolid && entity.direction.is_right()){
    entity.acceleration.left = entity.acceleration.right = 0.f;
    entity.position.x = (right_x - 1) * BlockBase::Size + Offset;
  }

  if (level.hitbox_grid[left_x][y] == Hitbox::NonSolid && entity.direction.is_left()){
    entity.acceleration.left = entity.acceleration.right = 0.f;
    entity.position.x = left_x * BlockBase::Size - Offset;
  }
};

static auto movement(EntityState& entity, const LevelState& level){
  if (entity.is_dead || !entity.is_active) return;

  auto left_boost = window::delta_time * entity.acceleration.left * EntityState::MovementSpeedMultiplier;
  auto right_boost = window::delta_time * entity.acceleration.right * EntityState::MovementSpeedMultiplier;

  detect_collision_with_level(entity, level, [&](const auto& collision_state){
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
static auto gravity_base(EntityState& entity, const LevelState& level, const Callable& collision_callback){
  if (entity.is_dead && entity.death_delay > 0.f) return;
  if (!entity.is_active) return;

  if (entity.is_on_ground && !entity.is_dead && entity.should_collide) entity.gravity = 0.f;
  entity.is_on_ground = false;

  entity.gravity += window::delta_time * EntityState::GravityForce * entity.gravity_boost;

  auto position_increaser = entity.gravity * window::delta_time * 70.f;

  static constexpr auto ViewHeight = LevelState::BlocksInColumn * BlockBase::Size;
  if (entity.position.y < level.camera_offset.y + ViewHeight || level.is_level_underground()){
    detect_collision_with_level(entity, level, [&](const auto& collision_state){
      collision_callback(collision_state, position_increaser);
    });
  }

  static constexpr auto MaxGravityForce = 100.f;

  position_increaser = std::min(position_increaser, MaxGravityForce);
  entity.position.y += position_increaser;
}

static auto gravity(EntityState& entity, const LevelState& level){
  gravity_base(entity, level, [&](const auto& collision_state, auto& position_increaser){
    if (entity.death_delay <= 0.f) return;

    if (collision_state.distance_above < -position_increaser){
      position_increaser = -collision_state.distance_above + 1.f;
      entity.gravity = 0.f;
    }

    using collision_controller::CollisionPadding;
    if (collision_state.distance_below == util::in_range(-CollisionPadding, position_increaser)){
      entity.is_on_ground = true;
      position_increaser = collision_state.distance_below;
    }
  });
}

static auto player_is_on_entity(const EntityState& entity, const PlayerState& player) -> bool{
  if (!entity.can_be_stomped) return false;

  if (collision_controller::intersects_in_x(player, entity)){
    const auto distance = entity.position.y - player.position.y - player.size.y;
    const auto hitbox_tolerance = entity.can_be_stomped
      ? -entity.size.y + BlockBase::Size / 6.f
      : -BlockBase::Size / 6.f;

    return distance == util::in_range(hitbox_tolerance, 0);
  }

  return false;
}

static auto kill_player_on_touch(const EntityState& entity, LevelState& level){
  if (terminal::god_mode) return;

  if (level.is_finished) return;

  if (!entity.is_active) return;
  if (entity.was_hit) return;
  if (entity.is_dead) return;

  auto& player = level.player;
  if (player.is_dead) return;
  if (player_is_on_entity(entity, player)) return;

  const auto entity_hitbox_offset = 1.f / 5.f;
  const auto entity_hitbox = collision_controller::Rect(
    entity.position + glm::vec2(entity.size * entity_hitbox_offset / 2.f),
    glm::vec2(entity.size * (1 - entity_hitbox_offset))
  );

  if (!collision_controller::intersects(player, entity_hitbox)) return;

  if (player.growth == PlayerState::Growth::Big){
    player.is_shrinking = true;
  }
  else if (!player.is_shrinking && player.invincibility_delay <= 0.f) { 
    player.is_dead = true; 
  }
};

static auto was_stomped(const EntityState& entity, const PlayerState& player) -> bool{
  if (!entity.can_be_stomped) return false;
  if (entity.was_hit) return false;
  if (entity.is_dead) return false;
  if (player.is_dead) return false;
  if (player.gravity < 0) return false;

  return player_is_on_entity(entity, player);
}

} //namespace mario::entity_controller
