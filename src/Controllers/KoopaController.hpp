#pragma once

#include "States/KoopaState.hpp"
#include "States/PlayerState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/ShellMonsterController.hpp"

static auto koopa_controller(KoopaState& koopa, LevelState& level){
  //Interaction with player
  auto& player = level.player;
  entity_die_when_hit_by_fireball(koopa, player, level.stats);
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
