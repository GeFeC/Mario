#pragma once

#include "Controllers/MonsterController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "States/MonsterState.hpp"
#include "States/LevelState.hpp"
#include <array>

namespace mario{

static auto run_controller(SpikeState& spike, LevelState& level){
  monster_controller::points_particles(spike);

  if (spike.position.y > (level.max_size().y + 1) * BlockBase::Size) return;

  entity_controller::gravity(spike, level);

  if (spike.can_move){
    entity_controller::movement(spike, level);
  }

  monster_controller::turn_around(spike);

  monster_controller::run_movement_animation(spike, textures::spike_walk);

  //Interaction with blocks
  monster_controller::die_when_on_bouncing_block(spike, level);

  //Interaction with player
  monster_controller::kill_player_on_touch(spike, level);
  monster_controller::become_active_when_seen(spike, level);
  monster_controller::die_when_hit_by_fireball(spike, level);
}

} //namespace mario
