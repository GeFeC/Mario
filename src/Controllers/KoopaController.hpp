#pragma once

#include "PolyControllers.hpp"

#include "States/KoopaState.hpp"
#include "States/PlayerState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/ShellMonsterController.hpp"

#include "Util/LoopedCounter.hpp"

static auto koopa_controller_base(KoopaState& koopa, LevelState& level){
  //Interaction with player
  auto& player = level.player;
  monster_die_when_hit_by_fireball(koopa, level);
  monster_become_active_when_seen(koopa, level);
  
  //Interaction with blocks
  monster_die_when_on_bouncing_block(koopa, level);
}

static auto green_koopa_controller(KoopaState& koopa, LevelState& level){
  shell_monster_controller(koopa, level, textures::green_koopa_walk);
  koopa_controller_base(koopa, level);
  shell_monster_handle_shell(
    koopa,
    level,
    textures::green_koopa_dead 
  );
}

static auto red_koopa_controller(KoopaState& koopa, LevelState& level){
  shell_monster_controller(koopa, level, textures::red_koopa_walk);
  koopa_controller_base(koopa, level);
  shell_monster_handle_shell(
    koopa,
    level,
    textures::red_koopa_dead 
  );
}

static auto purple_koopa_movement_controller(KoopaState& koopa, const LevelState& level){
  if (koopa.in_shell || koopa.vertical_flip == Drawable::Flip::UseFlip) return;

  if (level.purple_koopa_counter.value > 8.f){
    koopa.walk_speed = 10.f;
  }
  else{
    koopa.walk_speed = 3.f;
  }
  koopa.set_direction(koopa.direction);
}

static auto purple_koopa_controller(KoopaState& koopa, LevelState& level){
  shell_monster_controller(koopa, level, textures::purple_koopa_walk);
  koopa_controller_base(koopa, level);
  shell_monster_handle_shell(
    koopa,
    level,
    textures::purple_koopa_dead 
  );

  purple_koopa_movement_controller(koopa, level);
}

template<>
struct Controller<KoopaState>{
  static auto run(KoopaState& koopa, LevelState& level){
    using Type = KoopaState::Type;
    switch(koopa.type){
      case Type::Green: green_koopa_controller(koopa, level); return;
      case Type::Red: red_koopa_controller(koopa, level); return;
      case Type::Purple: purple_koopa_controller(koopa, level); return;
    }
  }
};
