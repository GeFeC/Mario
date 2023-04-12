#pragma once

#include "States/KoopaState.hpp"
#include "States/LoopedCounter.hpp"
#include "States/PlayerState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/ShellMonsterController.hpp"

static auto koopa_controller(KoopaState& koopa, LevelState& level){
  //Interaction with player
  auto& player = level.player;
  entity_die_when_hit_by_fireball(koopa, level);
  entity_become_active_when_seen(koopa, level);
  
  //Interaction with blocks
  entity_die_when_on_bouncing_block(koopa, level);
}

static auto green_koopa_controller(KoopaState& koopa, LevelState& level){
  shell_monster_controller(koopa, level, textures::green_koopa_walk);
  koopa_controller(koopa, level);
  entity_handle_shell(
    koopa,
    level,
    textures::green_koopa_dead 
  );
}

static auto red_koopa_controller(KoopaState& koopa, LevelState& level){
  shell_monster_controller(koopa, level, textures::red_koopa_walk);
  koopa_controller(koopa, level);
  entity_handle_shell(
    koopa,
    level,
    textures::red_koopa_dead 
  );
}

static auto purple_koopa_movement_controller(KoopaState& koopa, const LevelState& level){
  if (koopa.in_shell || koopa.vertical_flip == Drawable::Flip::UseFlip) return false;

  auto is_fast = false;
  if (level.purple_koopa_counter.value > 8.f){
    koopa.walk_speed = 10.f;
    is_fast = true;
  }
  else{
    koopa.walk_speed = 3.f;
  }
  koopa.set_direction(koopa.direction);

  return is_fast;
}

static auto purple_koopa_controller(KoopaState& koopa, LevelState& level){
  shell_monster_controller(koopa, level, textures::purple_koopa_walk);
  koopa_controller(koopa, level);
  entity_handle_shell(
    koopa,
    level,
    textures::purple_koopa_dead 
  );

  purple_koopa_movement_controller(koopa, level);
}
