#pragma once

#include "Controllers/MonsterController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "States/EntityState.hpp"
#include "States/MonsterState.hpp"
#include "States/LevelState.hpp"
#include "res/textures.hpp"
#include "config.hpp"

static auto shell_monster_controller(
    ShellMonsterState& entity, 
    const LevelState& level, 
    const std::array<Texture, 2>& walk_frames
){
  entity_gravity(entity, level);
  entity_movement(entity, level);
  monster_turn_around(entity);

  for (auto& p : entity.points_generator.items){
    points_particles_controller(p);
  }

  if (entity.is_dead || entity.in_shell) return;

  monster_run_movement_animation(entity, walk_frames);
}

static auto shell_monster_hide_in_shell(ShellMonsterState& entity, const Texture& dead_texture){
  entity.in_shell = true;
  entity.current_texture = &dead_texture;
  entity.acceleration.left = entity.acceleration.right = 0.f;
  entity.walk_speed = 0.f;

  const auto previous_size = entity.size.y;
  entity.size.y = entity.shell_height;

  entity.position.y += previous_size - entity.size.y;
}

static auto shell_monster_push_shell_on_player_touch(
    ShellMonsterState& entity, 
    LevelState& level
){
  auto& player = level.player;

  if (player.is_dead) return;
  if (entity.is_dead) return;
  if (!entity.should_collide) return;
  if (!collision::is_hovering(player, entity)) return;

  entity.shell_push_cooldown = glfwGetTime();
  entity.walk_speed = entity.shell_speed;

  if (entity.position.y > player.position.y + 10.f){
    entity.spawn_points(player.mobs_killed_in_row);

    const auto total_reward = entity.reward_for_killing * player.mobs_killed_in_row;
    level.stats.score += total_reward;
  }

  if (entity.position.x - player.position.x > 0){
    entity.set_direction(EntityState::DirectionRight);
    return;
  }

  entity.set_direction(EntityState::DirectionLeft);
};

static auto shell_monster_get_hitbox(const ShellMonsterState& entity){
  auto hitbox = ShellMonsterState();
  hitbox.position = entity.position + glm::vec2(0.f, entity.size.y - BlockBase::Size);
  hitbox.size = glm::vec2(BlockBase::Size);
  hitbox.is_dead = entity.is_dead;
  hitbox.is_active = entity.is_active;
  hitbox.should_collide = entity.should_collide;
  hitbox.vertical_flip = entity.vertical_flip;
  hitbox.was_hit = entity.was_hit;

  return hitbox;
}

static auto shell_monster_handle_shell(
    ShellMonsterState& entity, 
    LevelState& level,
    const Texture& dead_texture
){
  if (entity.in_shell){
    entity.fall_from_edge = true;
  }

  auto& player = level.player;
  if (entity.in_shell && entity.walk_speed == 0.f){
    shell_monster_push_shell_on_player_touch(entity, level);
    return;
  }

  auto entity_hitbox = shell_monster_get_hitbox(entity);
  if (glfwGetTime() - entity.shell_push_cooldown >= 0.2f){
    monster_die_when_stomped(entity, level, [&]{ 
      shell_monster_hide_in_shell(entity, dead_texture); 
    });
  }

  [&]{
    const auto distance = entity.position.x - player.position.x;
    if (distance > 0 && entity.acceleration.right == entity.shell_speed) return;
    if (distance < 0 && entity.acceleration.left == entity.shell_speed) return;

    if (entity.walk_speed > 0){
      monster_kill_player_on_touch(entity_hitbox, player);
    }
  }(); 

  if (entity.acceleration.left != entity.shell_speed && entity.acceleration.right != entity.shell_speed) return;

  auto shell_kill_entity = [&](MonsterState& target_entity){
    if (&target_entity == &entity) return;
    if (!target_entity.should_collide) return;
    if (!target_entity.is_active) return;
    if (entity.vertical_flip == Drawable::Flip::UseFlip) return;

    if (collision::is_hovering(entity, target_entity)){
      monster_bounce_die(target_entity, level.stats);
    }
  };

  //Killing Entities with shell
  level.game_objects.for_each_derived<MonsterState>([&](auto& target){
    if constexpr (std::is_convertible_v<decltype(target), MushroomState>) return;

    shell_kill_entity(target);
  });

  level.game_objects.for_each_template<QBlockState>([&](auto& block){
    if constexpr (std::decay_t<decltype(block)>::PusherType::ContainsEntity) {
      shell_kill_entity(block.pusher.entity);
    }
  });
};


