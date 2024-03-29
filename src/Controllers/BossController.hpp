#pragma once

#include "States/BossState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/MonsterController.hpp"

#include "Window.hpp"

namespace mario::boss_controller{

template<typename T>
static auto get_hitbox(BossState<T>& boss){
  auto boss_hitbox = MonsterState{ boss };
  boss_hitbox.is_active = true;
  boss_hitbox.size = boss.size - glm::vec2(0.5f, 0.5f);
  boss_hitbox.position = boss.position + glm::vec2(0.25f, 0.25f);

  return boss_hitbox;
}

template<typename T>
static auto take_damage(BossState<T>& boss){
  if (boss.hp == 0) return;

  boss.hp--;
  boss.blink_cooldown = 0.05f;
  boss.is_highlighted = true;
}

template<typename T>
static auto react_when_hit_by_fireball(BossState<T>& boss, LevelState& level){
	if (level.player.is_dead) return;

  monster_controller::react_when_hit_by_fireball(boss, level, [&](auto& fireball){
    take_damage(boss);
		fireball_controller::reset(fireball);
  });
}

template<typename T>
static auto handle_walking(BossState<T>& boss, const LevelState& level){
  if (boss.hp == 0) return;

  boss.position.x += boss.direction.as_int() * window::delta_time * boss.walk_speed * EntityState::MovementSpeedMultiplier;

  static constexpr auto LevelWidth = config::FrameBufferSize.x - BlockBase::Size;
  if (boss.position.x + boss.size.x >= LevelWidth) boss.direction = util::Direction::left();
  if (boss.position.x <= BlockBase::Size) boss.direction = util::Direction::right();

  static constexpr auto GroundPosition = config::FrameBufferSize.y - BlockBase::Size;
  boss.position.x = std::clamp(boss.position.x, BlockBase::Size, LevelWidth);
  boss.position.y = std::min(boss.position.y, GroundPosition);
}

template<typename T>
static auto run(BossState<T>& boss, LevelState& level){
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
  entity_controller::kill_player_on_touch(boss_hitbox, level);
  if (level.player.form == PlayerState::Form::Normal && !level.player.is_dead){
    level.player.is_dead = true;
		level.background_music->stop();
		sounds::sounds[sounds::LowTime].stop();
		sounds::sounds[sounds::Death].play();
  }
}

} //namespace mario::boss_controller
