#pragma once

#include "PolyControllers.hpp"

#include "Controllers/EntityController.hpp"
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
    entity_turn_around(spike);

    for (auto& p : spike.points_generator.items){
      points_particles_controller(p);
    }

    entity_run_movement_animation(spike, textures::spike_walk);

    //Interaction with blocks
    entity_die_when_on_bouncing_block(spike, level);

    //Interaction with player
    auto& player = level.player;
    entity_kill_player_on_touch(spike, player);
    entity_become_active_when_seen(spike, level);
    entity_die_when_hit_by_fireball(spike, level);
  }
};
