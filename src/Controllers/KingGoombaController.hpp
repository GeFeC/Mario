#pragma once

#include "Renderer/Drawable.hpp"
#include "States/BossState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/BossController.hpp"
#include "Window.hpp"
#include "config.hpp"
#include "res/textures.hpp"
#include "Util/Util.hpp"

namespace mario{

static auto run_controller(KingGoombaState& boss, LevelState& level){
  if (boss.hp > 0){
    monster_controller::run_movement_animation(boss, textures::king_goomba_walk);
  }

  //Jumping
  if (boss.is_on_ground){
    boss.gravity = KingGoombaState::JumpPower;
    boss.is_on_ground = false;
  }
  
  static constexpr auto GroundPosition = config::FrameBufferSize.y - BlockBase::Size;
  entity_controller::handle_gravity_base(boss, level, [&](const auto&, auto& position_increaser){
    if (boss.hp == 0.f) return;

    if (boss.position.y + boss.size.y >= GroundPosition){
      boss.is_on_ground = true;
    }
  });

  boss_controller::run(boss, level);
  boss_controller::handle_walking(boss, level);
  boss_controller::react_when_hit_by_fireball(boss, level);
}

} //namespace mario
