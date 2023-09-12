#pragma once

#include "States/BossState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/MonsterController.hpp"

#include "Window.hpp"

namespace mario::boss_controller{

static auto get_hitbox(BossState& boss){
  auto boss_hitbox = MonsterState{ boss };
  boss_hitbox.is_active = true;
  boss_hitbox.size = glm::vec2(2.f) * BlockBase::Size;
  boss_hitbox.position = glm::vec2(
    boss.position.x + boss.size.x / 2 - boss_hitbox.size.x / 2,
    boss.position.y + boss.size.y - 2.f * BlockBase::Size
  );

  return boss_hitbox;
}

static auto take_damage(BossState& boss){
  if (boss.hp == 0) return;

  boss.hp--;
  boss.blink_cooldown = 0.05f;
  boss.is_highlighted = true;
}

static auto react_when_hit_by_fireball(BossState& boss, LevelState& level){
  monster_controller::react_when_hit_by_fireball(boss, level, [&](auto& fireball){
    take_damage(boss);
    fireball.acceleration.left = fireball.acceleration.right = 0.f;
    fireball.position.x = util::BigValue;
  });
}

static auto walk(BossState& boss, const LevelState& level){
  if (boss.hp == 0) return;

  boss.position.x += boss.direction.as_int() * window::delta_time * boss.walk_speed * EntityState::MovementSpeedMultiplier;

  static constexpr auto LevelWidth = config::FrameBufferSize.x - BlockBase::Size;
  if (boss.position.x + boss.size.x >= LevelWidth) boss.direction = util::Direction::left();
  if (boss.position.x <= BlockBase::Size) boss.direction = util::Direction::right();

  static constexpr auto GroundPosition = config::FrameBufferSize.y - BlockBase::Size;
  boss.position.x = std::clamp(boss.position.x, BlockBase::Size, LevelWidth);
  boss.position.y = std::min(boss.position.y, GroundPosition);
}

static auto controller(BossState& boss, LevelState& level){
  if (boss.blink_cooldown <= 0.f){
    boss.is_highlighted = false;
  }

  //Bounce die
  if (boss.hp == 0 && boss.should_collide){
    boss.gravity = -25.f;
  }

  boss.blink_cooldown -= window::delta_time;

  if (boss.hp == 0){
    boss.vertical_flip = util::Flip::flip();
    boss.should_collide = false;
    boss.was_hit = true;
    level.is_finished = true;
    return; 
  }

  //Interactions with player
  auto boss_hitbox = get_hitbox(boss);
  entity_controller::kill_player_on_touch(boss_hitbox, level.player);
  if (level.player.form == PlayerState::Form::Normal){
    level.player.is_dead = true;
  }
}

} //namespace mario::boss_controller
