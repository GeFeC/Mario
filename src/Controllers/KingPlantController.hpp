#pragma once

#include "Controllers/EntityController.hpp"
#include "States/BossState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/BossController.hpp"
#include "Controllers/MonsterController.hpp"
#include "Controllers/BlackPlantController.hpp"
#include "res/textureGroups.hpp"

namespace mario{

static auto run_controller(KingPlantState& boss, LevelState& level){
  if (boss.offset <= 0.f){
    switch(util::random_value(0, 1)){
      case 0: 
        boss.position.x = (2.5f + util::random_value(0, 3) * 4.f) * BlockBase::Size;
        boss.position.y = 11.2f * BlockBase::Size;
        boss.vertical_flip = EntityState::Flip::NoFlip;
        break;
      
      case 1:
        boss.position.y = -KingPlantState::Size.y;
        boss.vertical_flip = EntityState::Flip::UseFlip;
        boss.position.x = (4.5f + util::random_value(0, 2) * 4.f) * BlockBase::Size;
        break;
    }
  }

  boss_controller::controller(boss, level);
  boss_controller::react_when_hit_by_fireball(boss, level);
  run_controller(boss | util::as<BlackPlantState&>, level);

  if (boss.hp == 0){
    entity_controller::gravity(boss, level);
  }
}

} //namespace mario
