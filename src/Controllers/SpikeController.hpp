#pragma once

#include "PolyControllers.hpp"

#include "Controllers/MonsterController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "States/MonsterState.hpp"
#include "States/LevelState.hpp"
#include <array>

template<>
struct Controller<SpikeState>{
  static auto run(
      SpikeState& spike, 
      LevelState& level
  ){
    entity_gravity(spike, level);
    entity_movement(spike, level);
    monster_turn_around(spike);
    monster_points_particles(spike);

    monster_run_movement_animation(spike, textures::spike_walk);

    //Interaction with blocks
    monster_die_when_on_bouncing_block(spike, level);

    //Interaction with player
    auto& player = level.player;
    monster_kill_player_on_touch(spike, player);
    monster_become_active_when_seen(spike, level);
    monster_die_when_hit_by_fireball(spike, level);
  }
};
