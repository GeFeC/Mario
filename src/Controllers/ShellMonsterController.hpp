#pragma once

#include "Controllers/MonsterController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "States/EntityState.hpp"
#include "States/MonsterState.hpp"
#include "States/LevelState.hpp"
#include "res/textures.hpp"
#include "config.hpp"

namespace mario::shell_monster_controller{

static auto run(
    ShellMonsterState& entity, 
    const LevelState& level, 
    const std::array<renderer::Texture, 2>& walk_frames
){
  entity_controller::handle_gravity(entity, level);
  entity_controller::handle_movement(entity, level);
  monster_controller::handle_turning_around(entity);

  monster_controller::handle_points_particles(entity);

  if (entity.is_dead || entity.in_shell) return;

  monster_controller::run_movement_animation(entity, walk_frames);
}

static auto hide_in_shell(ShellMonsterState& entity, const renderer::Texture& dead_texture){
  entity.in_shell = true;
  entity.current_texture = &dead_texture;
  entity.acceleration.left = entity.acceleration.right = 0.f;
  entity.walk_speed = 0.f;

  const auto previous_size = entity.size.y;
  entity.size.y = entity.shell_height;

  entity.position.y += (previous_size - entity.size.y) * entity.gravity_flip.as_binary();
}

static auto push_shell_on_player_touch(
    ShellMonsterState& entity, 
    LevelState& level
){
  auto& player = level.player;

  if (player.is_dead) return;
  if (entity.is_dead) return;
  if (!entity.should_collide) return;
  if (!collision_controller::intersects(player, entity)) return;

  entity.shell_push_cooldown = glfwGetTime();
  entity.walk_speed = entity.shell_speed;

  if (entity.position.y > player.position.y + 10.f){
    entity.spawn_points(player.mobs_killed_in_row);

    const auto total_reward = entity.reward_for_killing * player.mobs_killed_in_row;
    level.stats.score += total_reward;
  }

  if (entity.position.x - player.position.x > 0){
    entity.set_direction(util::Direction::right());
    return;
  }

  entity.set_direction(util::Direction::left());
};

static auto get_hitbox(const ShellMonsterState& entity){
  auto hitbox = entity;
  hitbox.position = entity.position + glm::vec2(0.f, entity.size.y - BlockBase::Size) * entity.gravity_flip.as_binary();
  hitbox.size = glm::vec2(BlockBase::Size);

  return hitbox;
}

static auto did_hit_monster_with_shell(
    const ShellMonsterState& monster, 
    const MonsterState& target 
){
  if (&target == &monster) return false;
  if (!target.should_collide) return false;
  if (!target.is_active) return false;
  if (monster.was_hit) return false;
  if (!monster.in_shell) return false;

  if (monster.acceleration.left != monster.shell_speed && monster.acceleration.right != monster.shell_speed) 
    return false;

  return collision_controller::intersects(monster, target);
}

static auto handle_shell(
    ShellMonsterState& entity, 
    LevelState& level,
    const renderer::Texture& dead_texture
){
  if (entity.in_shell){
    entity.fall_from_edge = true;
  }

  auto& player = level.player;
  if (entity.in_shell && entity.walk_speed == 0.f){
    push_shell_on_player_touch(entity, level);
    return;
  }

  if (glfwGetTime() - entity.shell_push_cooldown >= 0.2f){
    monster_controller::die_when_stomped(entity, level, [&]{ 
      hide_in_shell(entity, dead_texture); 
    });
  }

  [&]{
    const auto distance = entity.position.x - player.position.x;
    if (distance > 0 && entity.acceleration.right == entity.shell_speed) return;
    if (distance < 0 && entity.acceleration.left == entity.shell_speed) return;

    auto entity_hitbox = get_hitbox(entity);
    if (entity.walk_speed > 0){
      monster_controller::kill_player_on_touch(entity_hitbox, level);
    }
  }(); 

  auto shell_kill_entity = [&](MonsterState& target_entity){
    if (!did_hit_monster_with_shell(entity, target_entity)) return;

    monster_controller::bounce_die(target_entity, level.stats);
  };

  //Killing Entities with shell
  level.game_objects.for_each_derived<MonsterState>([&](auto& target){
    if constexpr (std::is_base_of_v<BossState, std::decay_t<decltype(target)>>) return;
    if constexpr (std::is_convertible_v<decltype(target), PlantState>) return;

    shell_kill_entity(target);
  });

  level.game_objects.for_each_template<QBlockState>([&](auto& block){
    if constexpr (std::decay_t<decltype(block)>::PusherType::ContainsEntity) {
      if constexpr (std::is_convertible_v<decltype(block.pusher.entity), MushroomState>) return;

      shell_kill_entity(block.pusher.entity);
    }
  });
};

} //namespace mario::shell_monster_controller
