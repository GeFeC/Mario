#pragma once

#include "PolyControllers.hpp"

#include "States/BeetleState.hpp"
#include "States/PlayerState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/ShellMonsterController.hpp"

template<>
struct Controller<BeetleState>{
  static auto run(BeetleState& beetle, LevelState& level){
    shell_monster_controller(beetle, level, textures::beetle_walk);

    //Interaction with blocks
    monster_die_when_on_bouncing_block(beetle, level);

    //Interaction with player
    auto& player = level.player;
    monster_endure_fireball(beetle, player);
    monster_become_active_when_seen(beetle, level);
    shell_monster_handle_shell(
      beetle,
      level,
      textures::beetle_dead
    );
  }
};
