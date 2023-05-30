#pragma once

#include "Controllers/EntityController.hpp"

static auto monster_kill_player_on_touch(const MonsterState& monster, PlayerState& player){
  if (monster.was_hit) return;
  entity_kill_player_on_touch(monster | util::as<EntityState>, player);
};

template<typename Function>
static auto monster_die_when_stomped(
    MonsterState& entity, 
    LevelState& level,
    Function set_entity_dead
){
  if (!entity.is_active) return false;

  auto& player = level.player;
  auto& stats = level.stats;

  if (entity_was_stomped(player, entity)){
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

static auto monster_become_active_when_seen(MonsterState& entity, const LevelState& level){
  if (entity.is_in_q_block) return;

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

static auto monster_bounce_out(MonsterState& entity){
  entity.gravity = MonsterState::BounceDiePower;
  entity.should_collide = false;
  entity.vertical_flip = Drawable::Flip::UseFlip;
}

static auto monster_bounce_die(MonsterState& entity, StatsState& stats){
  entity.was_hit = true;
  monster_bounce_out(entity);

  stats.score += entity.reward_for_killing;
  entity.points_generator.item().set_active(entity.reward_for_killing, entity.position);
}

static auto monster_is_hit_by_fireball(const MonsterState& entity, const FireballState& fireball){
  return collision::is_hovering(fireball, entity) && fireball.is_active && entity.is_active && entity.should_collide;
}

template<typename Callable>
static auto monster_react_when_hit_by_fireball(MonsterState& entity, LevelState& level, const Callable& callback){
  for (auto& fireball : level.player.fireballs){
    if (monster_is_hit_by_fireball(entity, fireball)){
      callback(fireball);
    }
  } 
}

static auto monster_die_when_hit_by_fireball(MonsterState& entity, LevelState& level){
  monster_react_when_hit_by_fireball(entity, level, [&](FireballState& fireball){
    monster_bounce_die(entity, level.stats);

    fireball.acceleration.left = fireball.acceleration.right = 0.f;
  });
};  

static auto monster_endure_fireball(MonsterState& entity, PlayerState& player){
  for (auto& fireball : player.fireballs){
    if (monster_is_hit_by_fireball(entity, fireball)){
      fireball.acceleration.left = fireball.acceleration.right = 0.f;
    }
  } 
};  

template<typename Reaction>
static auto monster_react_when_on_bouncing_block(
    MonsterState& entity, 
    const LevelState& level,
    Reaction reaction 
){
  const auto& objects = level.game_objects;

  const auto detect_bounce_and_react = [&](const auto& block){
    if (block.bounce_state.is_bouncing){
      if (entity.is_dead || !entity.should_collide) return;

      const auto collision_state = collision_controller(util::Rect(entity), util::Rect(block));
      
      if (collision_state.distance_below == util::in_range(-15.f, 0.f)){
        reaction();
      }
    }
  };

  for (auto& block : objects.get_vec<BricksBlockState>()){
    detect_bounce_and_react(block);
  }

  objects.for_each_template<QBlockState>(detect_bounce_and_react);
}

static auto monster_die_when_on_bouncing_block(MonsterState& entity, LevelState& level){
  monster_react_when_on_bouncing_block(entity, level, [&]{
    monster_bounce_die(entity, level.stats);
  });
}

//For mushrooms:
static auto monster_bounce_when_on_bouncing_block(MonsterState& entity, LevelState& level){
  monster_react_when_on_bouncing_block(entity, level, [&]{
    entity.gravity = MonsterState::BouncePower;
    entity.is_on_ground = false;
  });
}

static auto monster_run_movement_animation(MonsterState& entity, const std::array<Texture, 2>& walk_frames){
  const auto counter = glfwGetTime() * 8.f | util::as<int>;
  entity.current_texture = &walk_frames[counter % 2];
}

static auto monster_turn_around(MonsterState& entity){
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
