#pragma once

#include "PolyControllers.hpp"

#include "Renderer/Drawable.hpp"
#include "States/BossState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/BossController.hpp"
#include "Window.hpp"
#include "config.hpp"
#include "res/textures.hpp"
#include "Util/Util.hpp"

template<>
struct Controller<KingGoombaState>{
  static auto run(BossState& boss, LevelState& level){
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

    boss_controller(boss, level);
  }
};
