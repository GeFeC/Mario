#pragma once

#include "Renderer/Drawable.hpp"
#include "States/BossState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/EntityController.hpp"
#include "Window.hpp"
#include "config.hpp"
#include "res/textures.hpp"
#include "Util/Util.hpp"

static auto king_goomba_controller(KingGoombaState& boss, LevelState& level){
  if (boss.hp > 0){
    entity_run_movement_animation(boss, textures::king_goomba_walk);
  }

  //Jumping
  if (boss.is_on_ground){
    boss.gravity = KingGoombaState::JumpPower;
    boss.is_on_ground = false;
  }
  
  static constexpr auto GroundPosition = config::FrameBufferSize.y - BlockBase::Size;
  entity_gravity_base(boss, level, [&](const auto&, auto& position_increaser){
    if (boss.hp == 0.f) return;

    if (boss.position.y + boss.size.y >= GroundPosition){
      boss.is_on_ground = true;
    }
  });

  //Interactions with player
  static auto blink_cooldown = 0.f;
  if (blink_cooldown <= 0.f){
    boss.is_highlighted = false;
  }

  blink_cooldown -= window::delta_time;

  if (boss.hp == 0){
    boss.vertical_flip = Drawable::Flip::UseFlip;
    level.is_finished = true;
    return; 
  }

  auto boss_hitbox = MonsterState{};
  boss_hitbox.is_active = true;
  boss_hitbox.size = boss.size - glm::vec2(1) * BlockBase::Size;
  boss_hitbox.position = boss.position + glm::vec2((boss.size.x - boss_hitbox.size.x) / 2.f);

  entity_kill_player_on_touch(boss_hitbox, level.player);

  entity_react_when_hit_by_fireball(boss_hitbox, level, [&](auto& fireball){
    boss.hp--;
    blink_cooldown = 0.05f;
    boss.is_highlighted = true;
    fireball.acceleration.left = fireball.acceleration.right = 0.f;
  });

  if (level.player.form == PlayerState::Form::Normal){
    level.player.is_dead = true;
  }

  //Walking
  boss.position.x += boss.direction * window::delta_time * boss.walk_speed * EntityState::MovementSpeedMultiplier; 

  static constexpr auto LevelWidth = config::FrameBufferSize.x - BlockBase::Size;
  if (boss.position.x + boss.size.x >= LevelWidth) boss.direction = EntityState::DirectionLeft;
  if (boss.position.x <= BlockBase::Size) boss.direction = EntityState::DirectionRight;

  boss.position.x = std::clamp(boss.position.x, BlockBase::Size, LevelWidth);
  boss.position.y = std::min(boss.position.y, GroundPosition);
}
