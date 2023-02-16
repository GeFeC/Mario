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

  using config::BlockSize;
  const auto& position = entity.position;
  const auto& size = entity.size;

  for (const auto& block : level.blocks.normal){
    if (!block.is_solid) continue;

    const auto collision_state = collision_controller(util::Rect(entity), util::Rect(block));
    callable(collision_state);
  }

  if (entity.fall_from_edge) return;

  //Checking if entity should change direction not to fall from edge
  
  static constexpr auto Offset = 20.f;

  const auto x = (entity.position.x) / config::BlockSize;
  const auto y = (entity.position.y + entity.size.y + Offset) / config::BlockSize;
  const auto left_x = (entity.position.x + Offset) / config::BlockSize;
  const auto right_x = (entity.position.x + config::BlockSize - Offset) / config::BlockSize;

  if (!entity.is_on_ground) return;

  if (left_x + 1 >= config::MaxLevelSize || left_x - 1 <= 0) return;
  if (right_x + 1 >= config::MaxLevelSize || right_x - 1 <= 0) return;
  if (y >= config::BlocksInColumn || y - 1 <= 0) return;

  static constexpr auto Air = 0;
  if (level.hitbox_grid[right_x][y] == Air && entity.direction == EntityState::DirectionRight){
    entity.acceleration.left = entity.acceleration.right = 0.f;
  }

  if (level.hitbox_grid[left_x][y] == Air && entity.direction == EntityState::DirectionLeft){
    entity.acceleration.left = entity.acceleration.right = 0.f;
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

static auto entity_gravity(EntityState& entity, const LevelState& level){
  if (entity.is_dead && entity.death_delay > 0.f) return;
  if (!entity.is_active) return;

  if (entity.is_on_ground && !entity.is_dead && entity.should_collide) entity.gravity = 0.f;
  entity.is_on_ground = false;

  entity.gravity += window::delta_time * EntityState::GravityForce * entity.gravity_boost;

  auto position_increaser = entity.gravity * window::delta_time * 60;

  detect_entity_collision_with_level(entity, level, [&](const auto& collision_state){
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

  entity.position.y += position_increaser;
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

static auto entity_get_hitbox(EntityState& entity){
  auto hitbox = EntityState();
  hitbox.size = glm::min(glm::vec2(config::BlockSize), entity.size);
  hitbox.position = entity.position + glm::vec2(0, entity.size.y - hitbox.size.y);
  hitbox.is_dead = entity.is_dead;
  hitbox.should_collide = entity.should_collide;

  return hitbox;
}

static auto entity_kill_player_on_touch(EntityState& entity, PlayerState& player){
  if (entity.is_dead) return;
  if (!entity.should_collide) return;

  auto entity_hitbox = entity_get_hitbox(entity);

  if (!player_is_on_entity(player, entity) && collision::is_hovering(player, entity_hitbox)){
    if (player.growth == PlayerState::Growth::Big){
      player.is_shrinking = true;
    }
    else if (!player.is_shrinking && player.invincibility_delay <= 0.f) { 
      player.is_dead = true; 
    }
  }
};

template<typename Function>
static auto entity_die_when_stomped(
    MonsterState& entity, 
    PlayerState& player, 
    StatsState& stats,
    Function set_entity_dead
){
  auto entity_hitbox = entity_get_hitbox(entity);

  if (player_stomp_on_entity(player, entity_hitbox)){
    set_entity_dead();
    player.gravity = PlayerState::BouncePower;
    player.position.y = entity.position.y - player.size.y;

    const auto total_reward = entity.reward_for_killing * player.mobs_killed_in_row;
    stats.score += total_reward;

    entity.spawn_points(player.mobs_killed_in_row);

    ++player.mobs_killed_in_row;

    return true;
  }

  return false;
};

static auto entity_become_active_when_seen(MonsterState& entity, const PlayerState& player){
  const auto screen_scroll = config::PlayerPositionToScroll - player.position.x;
  const auto view_extension = std::max(screen_scroll, 0.f);

  if (entity.position.x - player.position.x <= config::BlocksInRow * config::BlockSize + view_extension){
    entity.is_active = true;
  }
};

static auto entity_bounce_die(MonsterState& entity, StatsState& stats){
  entity.gravity = MonsterState::BounceDiePower;
  entity.should_collide = false;
  entity.vertical_flip = Drawable::Flip::UseFlip;

  stats.score += entity.reward_for_killing;
  entity.points_manager.get_points_particles().set_active(entity.reward_for_killing, entity.position);
}

static auto entity_is_hit_by_fireball(MonsterState& entity, FireballState& fireball){
  return collision::is_hovering(fireball, entity) && fireball.is_active && entity.is_active && entity.should_collide;
}

static auto entity_die_when_hit_by_fireball(MonsterState& entity, PlayerState& player, StatsState& stats){
  for (auto& fireball : player.fireballs){
    if (entity_is_hit_by_fireball(entity, fireball)){
      entity_bounce_die(entity, stats);

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
    blocks.q_blocks
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
