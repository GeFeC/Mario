#pragma once

#include "Controllers/CollisionController.hpp"
#include "Renderer/Drawable.hpp"
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
  for (const auto& block : level.blocks.normal){
    if (!block.is_solid) continue;

    const auto collision_state = collision_controller(util::Rect(entity), util::Rect(block));
    callable(collision_state);
  }

  //Platforms
  util::multi_for([&](const auto& platform){
    const auto platform_rect = util::Rect(
      platform.position,
      platform.size()
    );

    const auto collision_state = collision_controller(util::Rect(entity), platform_rect);
    callable(collision_state);
  }, level.platforms, level.looped_platforms);

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

static auto entity_turn_around(MonsterState& entity){
  if (entity.acceleration.left == 0 && entity.acceleration.right == 0){
    entity.direction = -entity.direction;

    if (entity.direction == EntityState::DirectionLeft){
      entity.acceleration.left = entity.walk_speed;
    }

    if (entity.direction == EntityState::DirectionRight){
      entity.acceleration.right = entity.walk_speed;
    }
  }
}

static auto player_is_on_entity(const PlayerState& player, const EntityState& entity) -> bool{
  if (!entity.can_be_stomped) return false;

  if (collision::is_hovering_in_x(player, entity)){
    const auto distance = entity.position.y - player.position.y - player.size.y;
    return distance == util::in_range(-BlockBase::Size * 5.f/6.f, 0);
  }

  return false;
}

static auto entity_kill_player_on_touch(const EntityState& entity, PlayerState& player){
  if (entity.is_dead) return;
  if (player.is_dead) return;
  if (entity.vertical_flip == Drawable::Flip::UseFlip) return;
  if (player_is_on_entity(player, entity)) return;
  if (!collision::is_hovering(player, entity)) return;

  if (player.growth == PlayerState::Growth::Big){
    player.is_shrinking = true;
  }
  else if (!player.is_shrinking && player.invincibility_delay <= 0.f) { 
    player.is_dead = true; 
  }
};

static auto player_stomp_on_entity(const PlayerState& player, const EntityState& entity) -> bool{
  if (!entity.can_be_stomped) return false;
  if (!entity.should_collide) return false;
  if (entity.is_dead) return false;
  if (player.is_dead) return false;
  if (player.gravity < 0) return false;

  return player_is_on_entity(player, entity);
}

template<typename Function>
static auto entity_die_when_stomped(
    MonsterState& entity, 
    LevelState& level,
    Function set_entity_dead
){
  auto& player = level.player;
  auto& stats = level.stats;

  if (player_stomp_on_entity(player, entity)){
    set_entity_dead();
    player.gravity = PlayerState::BouncePower;
    player.position.y = entity.position.y - player.size.y;

    stats.score += entity.reward_for_killing * player.mobs_killed_in_row;

    entity.spawn_points(player.mobs_killed_in_row);

    ++player.mobs_killed_in_row;

    return true;
  }

  return false;
};

static auto entity_become_active_when_seen(EntityState& entity, const LevelState& level){
  const auto& player = level.player;

  const auto player_field_of_view_x = std::max(
    config::FrameBufferSize.x - LevelState::PlayerPositionToStartLevelScrolling.x,
    config::FrameBufferSize.x - player.position.x
  );

  const auto player_field_of_view_y = player.position.y - level.camera_offset.y;

  if (entity.position.x - player.position.x > player_field_of_view_x) return;
  if (player.position.y - entity.position.y - entity.size.y > player_field_of_view_y) return;

  entity.is_active = true;
};

static auto entity_bounce_out(MonsterState& entity){
  entity.gravity = MonsterState::BounceDiePower;
  entity.should_collide = false;
  entity.vertical_flip = Drawable::Flip::UseFlip;
}

static auto entity_bounce_die(MonsterState& entity, StatsState& stats){
  entity_bounce_out(entity);

  stats.score += entity.reward_for_killing;
  entity.points_generator.item().set_active(entity.reward_for_killing, entity.position);
}

static auto entity_is_hit_by_fireball(MonsterState& entity, FireballState& fireball){
  return collision::is_hovering(fireball, entity) && fireball.is_active && entity.is_active && entity.should_collide;
}

template<typename Callable>
static auto entity_react_when_hit_by_fireball(MonsterState& entity, LevelState& level, const Callable& callback){
  for (auto& fireball : level.player.fireballs){
    if (entity_is_hit_by_fireball(entity, fireball)){
      callback(fireball);
    }
  } 
}

static auto entity_die_when_hit_by_fireball(MonsterState& entity, LevelState& level){
  entity_react_when_hit_by_fireball(entity, level, [&](FireballState& fireball){
    entity_bounce_die(entity, level.stats);

    fireball.acceleration.left = fireball.acceleration.right = 0.f;
  });
};  

static auto entity_endure_fireball(MonsterState& entity, PlayerState& player){
  for (auto& fireball : player.fireballs){
    if (entity_is_hit_by_fireball(entity, fireball)){
      fireball.acceleration.left = fireball.acceleration.right = 0.f;
    }
  } 
};  

template<typename Reaction>
static auto entity_react_when_on_bouncing_block(
    MonsterState& entity, 
    const LevelState& level,
    Reaction reaction 
){
  const auto& blocks = level.blocks;

  util::multi_for([&](const auto& block){
    if (block.bounce_state.is_bouncing){
      if (entity.is_dead || !entity.should_collide) return;

      const auto collision_state = collision_controller(util::Rect(entity), util::Rect(block));
      
      if (collision_state.distance_below == util::in_range(-15.f, 0.f)){
        reaction();
      }
    }
  },
    blocks.bricks,
    blocks.q_blocks_with_coins,
    blocks.q_blocks_with_mushroom
  );
}

static auto entity_die_when_on_bouncing_block(MonsterState& entity, LevelState& level){
  entity_react_when_on_bouncing_block(entity, level, [&]{
    entity_bounce_die(entity, level.stats);
  });
}

//For mushrooms:
static auto entity_bounce_when_on_bouncing_block(MonsterState& entity, LevelState& level){
  entity_react_when_on_bouncing_block(entity, level, [&]{
    entity.gravity = MonsterState::BouncePower;
    entity.is_on_ground = false;
  });
}

static auto entity_run_movement_animation(MonsterState& entity, const std::array<Texture, 2>& walk_frames){
  const auto counter = glfwGetTime() * 8.f | util::as<int>;
  entity.current_texture = &walk_frames[counter % 2];
}
