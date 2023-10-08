#pragma once

#include "States/BeetleState.hpp"
#include "States/PlayerState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/ShellMonsterController.hpp"

namespace mario{

static auto run_controller(BeetleState& beetle, LevelState& level){
  shell_monster_controller::run(beetle, level, textures::beetle_walk);

  //Interaction with blocks
  monster_controller::die_when_on_bouncing_block(beetle, level);

  //Interaction with player
  auto& player = level.player;
  monster_controller::endure_fireball(beetle, player);
  monster_controller::become_active_when_seen(beetle, level);
  shell_monster_controller::handle_shell(
    beetle,
    level,
    textures::beetle_dead
  );
}

} //namespace mario
