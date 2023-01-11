#pragma once

#include "Controllers/CollisionController.hpp"
#include "Controllers/PlayerController.hpp"
#include "States/EntityState.hpp"
#include "States/LevelState.hpp"
#include "Util.hpp"
#include "Window.hpp"

static auto detect_entity_collision_with_level = [](EntityState& entity, const LevelState& level, auto callable){
  if (!entity.should_collide){
    return;
  }

  for (const auto& block : level.blocks.normal){
    if (!block.is_solid) continue;

    const auto collision_state = collision_controller(util::Rect(entity), util::Rect(block));
    callable(collision_state);
  }

  if (entity.fall_from_edge) return;

  for (const auto& block : level.blocks.entity_hitbox_blocks){
    if (!block.is_solid) continue;

    const auto collision_state = collision_controller(util::Rect(entity), util::Rect(block));
    callable(collision_state);
  }
};

template<typename F>
static auto entity_movement_helper(EntityState& entity, const LevelState& level, const F& detect_collisions){
  if (entity.is_dead || !entity.is_active) return;

  auto left_boost = window::delta_time * entity.acceleration.left * 100;
  auto right_boost = window::delta_time * entity.acceleration.right * 100;

  detect_collisions(entity, level, [&](const auto& collision_state){
    if (collision_state.distance_left < left_boost){
      left_boost = collision_state.distance_left;
      entity.acceleration.left = 0.f;
    }
    if (collision_state.distance_right < right_boost){
      right_boost = collision_state.distance_right;
      entity.acceleration.right = 0.f;
    }
  });

  entity.position.x += right_boost;
  entity.position.x -= left_boost;
}

static auto entity_movement(EntityState& entity, const LevelState& level){
  entity_movement_helper(entity, level, detect_entity_collision_with_level);
}

static auto entity_gravity(EntityState& entity, const LevelState& level, int gravity_boost = 1){
  if (entity.is_dead && entity.death_delay > 0.f) return;
  if (!entity.is_active) return;

  if (entity.is_on_ground && !entity.is_dead && entity.should_collide) entity.gravity = 0.f;
  entity.is_on_ground = false;

  entity.gravity += window::delta_time * 50 * gravity_boost;

  auto position_increaser = entity.gravity * window::delta_time * 60;

  detect_entity_collision_with_level(entity, level, [&](const auto& collision_state){
    if (entity.death_delay <= 0.f) return;

    if (collision_state.distance_below < position_increaser && collision_state.distance_below > -CollisionOffset){
      entity.is_on_ground = true;
      position_increaser = collision_state.distance_below;
    }
  });

  entity.position.y += position_increaser;
}

static auto entity_turn_around(EntityState& entity, int speed){
  if (entity.acceleration.left == 0 && entity.acceleration.right == 0){
    entity.direction = -entity.direction;

    if (entity.direction == EntityState::DirectionLeft){
      entity.acceleration.left = speed;
    }

    if (entity.direction == EntityState::DirectionRight){
      entity.acceleration.right = speed;
    }
  }
}

static auto entity_kill_player_on_touch(EntityState& entity, const util::Rect& entity_hitbox_rect, PlayerState& player){
  auto entity_hitbox = EntityState();
  entity_hitbox.position = entity_hitbox_rect.position;
  entity_hitbox.size = entity_hitbox_rect.size;

  if (!player_stomp_on_entity(player, entity) && collision::is_hovering(player, entity_hitbox) && entity.can_kill()){
    if (player.growth == PlayerState::Growth::Big){
      player.is_shrinking = true;
    }
    else if (!player.is_shrinking && player.invincibility_delay <= 0.f) { 
      player.is_dead = true; 
    }
  }
};

static auto entity_kill_player_on_touch(MonsterState& entity, PlayerState& player){
  entity_kill_player_on_touch(entity, util::Rect(entity), player);
};

template<typename Function>
static auto entity_die_when_stomped(
    MonsterState& entity, 
    const util::Rect& hitbox, 
    PlayerState& player, 
    LevelState& level,
    int reward, 
    Function set_entity_dead
){
  auto entity_hitbox = EntityState();
  entity_hitbox.position = hitbox.position;
  entity_hitbox.size = hitbox.size;
  entity_hitbox.is_dead = entity.is_dead;

  if (player_stomp_on_entity(player, entity_hitbox) && !player.is_dead && !entity.is_dead && entity.should_collide){
    set_entity_dead();
    player.gravity = -15.f;

    level.stats_state.score += reward * player.mobs_killed_in_row;
    entity.points_manager.make_next_points_particles();
    entity.points_manager.get_points_particles().set_active(reward * player.mobs_killed_in_row, entity.position);

    ++player.mobs_killed_in_row;

    return true;
  }

  return false;
};

template<typename Function>
static auto entity_die_when_stomped(
    MonsterState& entity, 
    PlayerState& player, 
    LevelState& level, 
    int reward, 
    Function set_entity_dead
){
  entity_die_when_stomped(entity, util::Rect(entity), player, level, reward, set_entity_dead);
};

static auto entity_become_active_when_seen(MonsterState& entity, const PlayerState& player){
  const auto screen_scroll = config::PlayerPositionToScroll - player.position.x;
  const auto view_extension = std::max(screen_scroll, 0.f);

  if (entity.position.x - player.position.x <= config::BlocksInRow * config::BlockSize + view_extension){
    entity.is_active = true;
  }
};

static auto entity_bounce_die(MonsterState& entity, LevelState& level, int reward){
  entity.gravity = -20.f;
  entity.should_collide = false;
  entity.vertical_flip = Drawable::Flip::UseFlip;

  level.stats_state.score += reward;
  entity.points_manager.get_points_particles().set_active(reward, entity.position);
}

static auto entity_is_hit_by_fireball(MonsterState& entity, FireballState& fireball){
  return collision::is_hovering(fireball, entity) && fireball.is_active && entity.is_active && entity.should_collide;
}

static auto entity_die_when_hit_by_fireball(MonsterState& entity, PlayerState& player, LevelState& level, int reward){
  for (auto& fireball : player.fireballs){
    if (entity_is_hit_by_fireball(entity, fireball)){
      entity_bounce_die(entity, level, reward);

      fireball.acceleration.left = fireball.acceleration.right = 0.f;
    }
  } 
};  

static auto entity_endure_fireball(MonsterState& entity, PlayerState& player){
  for (auto& fireball : player.fireballs){
    if (entity_is_hit_by_fireball(entity, fireball)){
      fireball.acceleration.left = fireball.acceleration.right = 0.f;
    }
  } 
};  
