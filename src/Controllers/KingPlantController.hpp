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
  static constexpr auto LowerPosition = 0;
  static constexpr auto UpperPosition = 1;

  //Get random position
  if (boss.offset <= 0.f){
    switch(util::random_value(0, 1)){
      case LowerPosition: 
        boss.position.x = (2.75f + util::random_value(0, 3) * 4.f) * BlockBase::Size;
        boss.position.y = 11.2f * BlockBase::Size;
        boss.vertical_flip = util::Flip::no_flip();
        break;
      
      case UpperPosition:
        boss.position.x = (4.75f + util::random_value(0, 2) * 4.f) * BlockBase::Size;
        boss.position.y = -KingPlantState::Size.y;
        boss.vertical_flip = util::Flip::flip();
        break;
    }
  }

  //Boss controller
  boss_controller::run(boss, level);
  boss_controller::react_when_hit_by_fireball(boss, level);
  run_controller(boss | util::as<BlackPlantState&>, level);

  if (boss.hp == 0){
    entity_controller::handle_gravity(boss, level);
  }
}

} //namespace mario
