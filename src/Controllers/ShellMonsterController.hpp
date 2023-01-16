#pragma once

#include "Controllers/EntityController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "States/EntityState.hpp"
#include "States/MonsterState.hpp"
#include "States/LevelState.hpp"
#include "res/textures.hpp"
#include "config.hpp"

static auto shell_monster_controller(
    ShellMonsterState& entity, 
    LevelState& level, 
    const std::array<Texture, 2>& walk
){
  entity_gravity(entity, level);
  entity_movement(entity, level);
  entity_turn_around(entity, entity.current_walk_speed);

  for (auto& p : entity.points_manager.points){
    points_particles_controller(p);
  }

  if (entity.is_dead || entity.in_shell) return;

  const auto counter = static_cast<int>(glfwGetTime() * 8.f);
  entity.current_texture = &walk[counter % 2];
}

static auto shell_monster_hide_in_shell(ShellMonsterState& entity, float new_size, Texture const* dead_texture){
  entity.in_shell = true;
  entity.current_texture = dead_texture;
  entity.acceleration.left = entity.acceleration.right = 0.f;
  entity.current_walk_speed = 0.f;

  const auto previous_size = entity.size.y;
  entity.size.y = new_size;

  entity.position.y += previous_size - entity.size.y;
}

static auto entity_push_shell_on_player_touch(
    ShellMonsterState& entity, 
    PlayerState& player, 
    LevelState& level, 
    int reward
){
  if (!collision::is_hovering(player, entity) || entity.is_dead || !entity.should_collide) return;

  entity.shell_push_cooldown = glfwGetTime();
  entity.current_walk_speed = config::KoopaShellWalkSpeed;

  if (entity.position.y > player.position.y + 10.f){
    entity.points_manager.make_next_points_particles();
    auto& points = entity.points_manager.get_points_particles();

    const auto total_reward = reward * player.mobs_killed_in_row;
    points.set_active(total_reward, entity.position);
    level.stats.score += total_reward;
  }

  if (entity.position.x - player.position.x > 0){
    entity.set_direction(EntityState::DirectionRight, config::KoopaShellWalkSpeed);
    return;
  }

  entity.set_direction(EntityState::DirectionLeft, config::KoopaShellWalkSpeed);
};

static auto entity_handle_shell(
    ShellMonsterState& entity, 
    PlayerState& player,
    LevelState& level,
    int reward, 
    int shell_speed,
    float new_size,
    const Texture& dead_texture
){
  entity_become_active_when_seen(entity, player);

  if (entity.in_shell){
    entity.fall_from_edge = true;
  }

  if (entity.in_shell && entity.current_walk_speed == 0.f){
    entity_push_shell_on_player_touch(entity, player, level, reward);
    return;
  }

  const auto entity_hitbox = entity.in_shell
  ? util::Rect(entity.position, glm::vec2(config::BlockSize))
  : util::Rect(
      entity.position + glm::vec2(0, entity.size.y - new_size), 
      glm::vec2(config::BlockSize)
    );

  if (glfwGetTime() - entity.shell_push_cooldown >= 0.2f){
    entity_die_when_stomped(entity, entity_hitbox, player, level, reward, [&]{ 
      shell_monster_hide_in_shell(entity, new_size, &dead_texture); 
    });
  }

  [&]{
    const auto distance = entity.position.x - player.position.x;
    if (distance > 0 && entity.acceleration.right == shell_speed) return;
    if (distance < 0 && entity.acceleration.left == shell_speed) return;

    if (entity.current_walk_speed > 0){
      entity_kill_player_on_touch(entity, entity_hitbox, player);
    }
  }(); 

  if (entity.acceleration.left != shell_speed && entity.acceleration.right != shell_speed) return;

  auto shell_kill_entity = [&](MonsterState& target_entity, int reward){
    if (&target_entity == &entity) return;

    if (collision::is_hovering(entity, target_entity) && target_entity.should_collide && target_entity.is_active){
      entity_bounce_die(target_entity, level, reward);
    }
  };

  //Killing Entities with shell
  auto& entities = level.entities;
  for (auto& target : entities.goombas){
    shell_kill_entity(target, config::RewardForKillingGoomba);
  }

  for (auto& target : entities.red_goombas){
    shell_kill_entity(target, config::RewardForKillingGoomba);
  }

  for (auto& target : entities.yellow_goombas){
    shell_kill_entity(target, config::RewardForKillingFastGoomba);
  }

  for (auto& target : entities.green_koopas){
    shell_kill_entity(target, config::RewardForKillingKoopa);
  }

  for (auto& target : entities.red_koopas){
    shell_kill_entity(target, config::RewardForKillingKoopa);
  }

  for (auto& target : entities.beetles){
    shell_kill_entity(target, config::RewardForKillingBeetle);
  }
};


