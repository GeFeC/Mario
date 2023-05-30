#pragma once

#include "States/BossState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/MonsterController.hpp"

#include "Window.hpp"

static auto boss_get_hitbox(BossState& boss){
  auto boss_hitbox = MonsterState{ boss };
  boss_hitbox.is_active = true;
  boss_hitbox.size = glm::vec2(2.f) * BlockBase::Size;
  boss_hitbox.position = glm::vec2(
    boss.position.x + boss.size.x / 2 - boss_hitbox.size.x / 2,
    boss.position.y + boss.size.y - 2.f * BlockBase::Size
  );

  return boss_hitbox;
}

static auto boss_take_damage(BossState& boss){
  boss.hp--;
  boss.blink_cooldown = 0.05f;
  boss.is_highlighted = true;
}

static auto boss_react_when_hit_by_fireball(BossState& boss, LevelState& level){
  auto boss_hitbox = boss_get_hitbox(boss);

  monster_react_when_hit_by_fireball(boss_hitbox, level, [&](auto& fireball){
    boss_take_damage(boss);
    fireball.acceleration.left = fireball.acceleration.right = 0.f;
  });
}

static auto boss_controller(BossState& boss, LevelState& level){
  if (boss.blink_cooldown <= 0.f){
    boss.is_highlighted = false;
  }

  //Bounce die
  if (boss.hp == 0 && boss.should_collide){
    boss.gravity = -25.f;
  }

  boss.blink_cooldown -= window::delta_time;

  if (boss.hp == 0){
    boss.vertical_flip = Drawable::Flip::UseFlip;
    boss.should_collide = false;
    level.is_finished = true;
    return; 
  }

  //Interactions with player
  auto boss_hitbox = boss_get_hitbox(boss);
  entity_kill_player_on_touch(boss_hitbox, level.player);
  if (level.player.form == PlayerState::Form::Normal){
    level.player.is_dead = true;
  }

  //Walking
  boss.position.x += boss.direction * window::delta_time * boss.walk_speed * EntityState::MovementSpeedMultiplier;

  static constexpr auto LevelWidth = config::FrameBufferSize.x - BlockBase::Size;
  if (boss.position.x + boss.size.x >= LevelWidth) boss.direction = EntityState::DirectionLeft;
  if (boss.position.x <= BlockBase::Size) boss.direction = EntityState::DirectionRight;

  static constexpr auto GroundPosition = config::FrameBufferSize.y - BlockBase::Size;
  boss.position.x = std::clamp(boss.position.x, BlockBase::Size, LevelWidth);
  boss.position.y = std::min(boss.position.y, GroundPosition);
}
