#pragma once

#include "States/KoopaState.hpp"
#include "States/PlayerState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/ShellMonsterController.hpp"

#include "Util/LoopedCounter.hpp"

namespace mario::koopa_controller{

static auto controller_base(KoopaState& koopa, LevelState& level){
  //Interaction with player
  monster_controller::die_when_hit_by_fireball(koopa, level);
  monster_controller::become_active_when_seen(koopa, level);
  
  //Interaction with blocks
  monster_controller::die_when_on_bouncing_block(koopa, level);
}

static auto green_koopa_controller(KoopaState& koopa, LevelState& level){
  shell_monster_controller::controller(koopa, level, textures::green_koopa_walk);
  controller_base(koopa, level);
  shell_monster_controller::handle_shell(
    koopa,
    level,
    textures::green_koopa_dead 
  );
}

static auto red_koopa_controller(KoopaState& koopa, LevelState& level){
  shell_monster_controller::controller(koopa, level, textures::red_koopa_walk);
  controller_base(koopa, level);
  shell_monster_controller::handle_shell(
    koopa,
    level,
    textures::red_koopa_dead 
  );
}

static auto purple_koopa_speedup(KoopaState& koopa, const LevelState& level){
  if (koopa.in_shell || koopa.vertical_flip == EntityState::Flip::UseFlip) return;

  if (level.purple_koopa_counter.value > 8.f){
    koopa.walk_speed = 10.f;
  }
  else{
    koopa.walk_speed = 3.f;
  }
  koopa.set_direction(koopa.direction);
}

static auto purple_koopa_controller(KoopaState& koopa, LevelState& level){
  shell_monster_controller::controller(koopa, level, textures::purple_koopa_walk);
  controller_base(koopa, level);
  shell_monster_controller::handle_shell(
    koopa,
    level,
    textures::purple_koopa_dead 
  );

  purple_koopa_speedup(koopa, level);
}

} //namespace mario::koopa_controller

namespace mario{

static auto run_controller(KoopaState& koopa, LevelState& level){
  using Type = KoopaState::Type;
  switch(koopa.type){
    case Type::Green: koopa_controller::green_koopa_controller(koopa, level); return;
    case Type::Red: koopa_controller::red_koopa_controller(koopa, level); return;
    case Type::Purple: koopa_controller::purple_koopa_controller(koopa, level); return;
  }
}

} //namespace mario
