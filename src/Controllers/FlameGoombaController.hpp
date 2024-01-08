#pragma once

#include "Controllers/FlameController.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/MonsterController.hpp"
#include "States/FlameGoombaState.hpp"
#include "States/LevelState.hpp"
#include "Util/Util.hpp"
#include "res/textures.hpp"
#include "Window.hpp"

namespace mario{

static auto run_controller(FlameGoombaState& goomba, LevelState& level){
  //Inner flame:
  goomba.inner_flame.position = {
    goomba.position.x + goomba.size.x / 2.f - goomba.inner_flame.particle_size / 2.f,
    goomba.position.y + goomba.size.y - goomba.inner_flame.particle_size
  };

  flame_controller(goomba.inner_flame, level);

  //Setting on fire everything around:
  goomba.flame_put_counter.run(window::delta_time, [&]{
    goomba.flames_generator.make_item_if_needed();
    auto& new_flame = goomba.flames_generator.item();
    new_flame = FlameState();
    new_flame.is_active = true;
    new_flame.position = goomba.inner_flame.position;
    new_flame.particle_counter.max_loops = 20;
  });

  for (auto& flame : goomba.flames_generator.items){
    flame_controller(flame, level, 10 - flame.particle_counter.loops / 2.f);

    if (flame.particle_counter.stopped_counting()){
      flame.is_active = false;
      flame.position = glm::vec2(util::BigValue);
    }
  }

  //Base:
	monster_controller::die_when_on_bouncing_block(goomba, level);
  monster_controller::endure_fireball(goomba, level.player);
  monster_controller::become_active_when_seen(goomba, level);
  monster_controller::handle_points_particles(goomba);
  entity_controller::run_movement_animation(goomba, textures::flame_goomba_walk);
  monster_controller::handle_turning_around(goomba);
  monster_controller::kill_player_on_touch(goomba, level);
  entity_controller::handle_gravity(goomba, level);
  entity_controller::handle_movement(goomba, level);
}

} //namespace mario
