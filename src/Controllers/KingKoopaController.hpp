#pragma once

#include "States/BossState.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/BossController.hpp"
#include "Util/Util.hpp"

static auto king_koopa_reset(KingKoopaState& boss){
  boss.in_shell = false;

  const auto previous_height = boss.size.y;
  boss.size.y = KingKoopaState::Size.y;
  boss.position.y += previous_height - boss.size.y;

  boss.walk_speed = BossState::WalkSpeed;
  boss.withdraw_delay = KingKoopaState::get_withdraw_delay();
  boss.speedup_delay = KingKoopaState::SpeedUpDelay;
  boss.withdraw_duration = KingKoopaState::WithdrawDuration;
  boss.current_texture = &textures::green_koopa_walk[0];
};

static auto king_koopa_controller(KingKoopaState& boss, LevelState& level){
  if (boss.hp == 0) king_koopa_reset(boss);

  //Speeding up
  if (boss.withdraw_delay <= 0.f){
    boss.in_shell = true;

    if (boss.speedup_delay > 0.f) {
      boss.walk_speed = 0.f;
      boss.withdraw_duration = KingKoopaState::WithdrawDuration;
    }
    else{
      boss.walk_speed = 20.f;
      boss.withdraw_duration -= window::delta_time;
    }

    //Set texture and size
    boss.current_texture = &textures::green_koopa_dead;
    const auto previous_height = boss.size.y;
    boss.size.y = boss.size.x * 7.f / 8.f;
    boss.position.y += previous_height - boss.size.y;

    boss.speedup_delay -= window::delta_time;

    if (boss.withdraw_duration <= 0.f){
      boss.in_shell = false;

      king_koopa_reset(boss);
    }
  }

  boss.withdraw_delay -= window::delta_time;

  //Movement
  if (boss.hp > 0 && !boss.in_shell){
    entity_run_movement_animation(boss, textures::green_koopa_walk);
  }

  boss_controller(boss, level);
  entity_gravity(boss, level);
}
