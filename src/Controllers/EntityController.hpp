#pragma once

#include "Controllers/CollisionController.hpp"
#include "States/BlockState.hpp"
#include "States/EntityState.hpp"
#include "States/LevelState.hpp"
#include "States/MonsterState.hpp"
#include "Util/Loop.hpp"
#include "Util/Util.hpp"
#include "Window.hpp"

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

    const auto collision_state = collision_controller::run(
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

    const auto collision_state = collision_controller::run(
      collision_controller::Rect(entity), 
      platform_rect
    );

    callable(collision_state);
  });

  if (entity.fall_from_edge) return;
  if (!entity.is_on_ground) return;
  if (entity.is_on_platform) return;

  //Checking if entity should change direction not to fall from edge
  const auto Offset = MonsterState::EdgeDetectionOffset;

  const auto entity_bottom_position = entity.position.y 
    + entity.size.y * entity.gravity_flip.as_binary()
    + collision_controller::CollisionPadding * entity.gravity_flip.as_int();

  const int y = entity_bottom_position / BlockBase::Size;

  const int left_x = (entity.position.x + Offset) / BlockBase::Size;
  const int right_x = (entity.position.x + BlockBase::Size - Offset) / BlockBase::Size;

  const auto level_size = level.max_size();

  if (left_x >= level_size.x || left_x < 0) return;
  if (right_x >= level_size.x || right_x < 0) return;
  if (y >= level_size.y || y <= entity.gravity_flip.as_int()) return;

  using Hitbox = LevelState::HitboxState;

  if (level.hitbox_grid[right_x][y] == Hitbox::NonSolid && entity.direction.is_right()){
    entity.position.x = (right_x - 1) * BlockBase::Size + Offset;

		if (!entity.follows_player){
			entity.turn_around();
		}
  }

  if (level.hitbox_grid[left_x][y] == Hitbox::NonSolid && entity.direction.is_left()){
    entity.position.x = (left_x + 1) * BlockBase::Size - Offset;

		if (!entity.follows_player){
			entity.turn_around();
		}
  }
};

static auto handle_movement(EntityState& entity, const LevelState& level){
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

static auto run_movement_animation(EntityState& entity, const std::array<renderer::Texture, 2>& frames, float speed = 8.f){
  const auto counter = glfwGetTime() * speed | util::as<int>;
  entity.current_texture = &frames[counter % 2];
}

template<typename Callable>
static auto handle_gravity_base(EntityState& entity, const LevelState& level, const Callable& collision_callback){
  if (entity.is_dead && entity.death_delay > 0.f) return;
  if (!entity.is_active) return;

  if (entity.is_on_ground && !entity.is_dead && entity.should_collide) entity.gravity = 0.f;
  entity.is_on_ground = false;

  entity.gravity += window::delta_time * EntityState::GravityForce * entity.gravity_boost;

  auto position_increaser = entity.gravity * window::delta_time * 70.f;

	detect_collision_with_level(entity, level, [&](const auto& collision_state){
		collision_callback(collision_state, position_increaser);
	});

  static constexpr auto MaxGravityForce = 100.f;

  position_increaser = std::min(position_increaser, MaxGravityForce);
  entity.position.y += position_increaser * entity.gravity_flip.as_int();
}

static auto handle_gravity(EntityState& entity, const LevelState& level){
  handle_gravity_base(
      entity, 
      level, 
      [&](const collision_controller::CollisionState& collision_state, auto& position_increaser_ref){
    if (entity.death_delay <= 0.f) return;

    const auto position_increaser = position_increaser_ref * entity.gravity_flip.as_int();

    using collision_controller::CollisionPadding;
    if (collision_state.distance_below == util::in_range(-CollisionPadding, position_increaser)){
      if (!entity.gravity_flip.is_flipped()) entity.is_on_ground = true;

      position_increaser_ref = collision_state.distance_below * entity.gravity_flip.as_int();
      entity.gravity = 0;
    }

    if (collision_state.distance_above == util::BigValue) return;

    if (-collision_state.distance_above == util::in_range(position_increaser, CollisionPadding)){
      if (entity.gravity_flip.is_flipped()) entity.is_on_ground = true;

      position_increaser_ref = -collision_state.distance_above * entity.gravity_flip.as_int();
      entity.gravity = 0;
    }
  });
}

static auto is_player_on_entity(const EntityState& entity, const PlayerState& player) -> bool{
  if (!entity.can_be_stomped) return false;
  if (entity.gravity_flip != player.gravity_flip) return false;

  if (collision_controller::intersects_in_x(player, entity)){
    const auto& gravity_flip = entity.gravity_flip;

    const auto distance = gravity_flip.is_flipped()
      ? player.position.y - entity.position.y - entity.size.y
      : entity.position.y - player.position.y - player.size.y;

    const auto hitbox_tolerance = entity.can_be_stomped
      ? entity.size.y - BlockBase::Size / 6.f
      : BlockBase::Size / 6.f;

    return distance == util::in_range(-hitbox_tolerance, 0);
  }

  return false;
}

static auto kill_player_on_touch(const EntityState& entity, LevelState& level){
  if (terminal::god_mode) return false;

  if (level.is_finished) return false;

  if (!entity.is_active) return false;
  if (entity.was_hit) return false;
  if (entity.is_dead) return false;

  auto& player = level.player;
  if (player.is_dead) return false;
  if (is_player_on_entity(entity, player)) return false;

  static constexpr auto EntityHitboxOffset = 1.f / 5.f;
  const auto entity_hitbox = collision_controller::Rect(
    entity.position + entity.size * EntityHitboxOffset / 2.f,
    entity.size * (1 - EntityHitboxOffset)
  );

  if (!collision_controller::intersects(player, entity_hitbox)) return false;

  if (player.growth == PlayerState::Growth::Big){
    player.is_shrinking = true;
		return true;
  }
  else if (!player.is_shrinking && player.invincibility_delay <= 0.f) { 
    player.is_dead = true; 
		return true;
  }

	return false;
};

static auto was_stomped(const EntityState& entity, const PlayerState& player) -> bool{
  if (!entity.can_be_stomped) return false;
  if (entity.was_hit) return false;
  if (entity.is_dead) return false;
  if (player.is_dead) return false;
  if (player.gravity < 0) return false;

  return is_player_on_entity(entity, player);
}

} //namespace mario::entity_controller
