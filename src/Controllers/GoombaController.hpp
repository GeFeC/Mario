#pragma once

#include "Controllers/MonsterController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "States/LevelState.hpp"
#include "res/textures.hpp"
#include "Window.hpp"

#include <GLFW/glfw3.h>

namespace mario::goomba_controller{

static auto set_dead(GoombaState& goomba, const renderer::Texture& dead_texture){
  goomba.is_dead = true;
  goomba.current_texture = &dead_texture;
}

static auto run_walk_animation(GoombaState& goomba, const std::array<renderer::Texture, 2>& walk_frames){
  if (goomba.is_dead) return;
  monster_controller::run_movement_animation(goomba, walk_frames);
}

static auto controller_base(GoombaState& goomba, LevelState& level){
  //Interactions with player
  monster_controller::kill_player_on_touch(goomba, level);
  monster_controller::become_active_when_seen(goomba, level);
  monster_controller::die_when_hit_by_fireball(goomba, level);

  //Interaction with blocks
  monster_controller::die_when_on_bouncing_block(goomba, level);

  entity_controller::gravity(goomba, level);
  entity_controller::movement(goomba, level);
  monster_controller::turn_around(goomba);
  monster_controller::points_particles(goomba);

  if (goomba.is_dead){
    goomba.death_delay -= window::delta_time;

    if (goomba.death_delay <= 0.f){
      goomba.is_visible = false;
    }

    return;
  } 
}

static auto normal_goomba_controller(GoombaState& goomba, LevelState& level){
  controller_base(goomba, level);
  run_walk_animation(goomba, textures::goomba_walk);

  monster_controller::die_when_stomped(goomba, level, [&]{ 
    set_dead(goomba, textures::goomba_dead);
  });
}

static auto red_goomba_controller(GoombaState& goomba, LevelState& level){
  controller_base(goomba, level);
  run_walk_animation(goomba, textures::red_goomba_walk);

  monster_controller::die_when_stomped(goomba, level, [&]{ 
    set_dead(goomba, textures::red_goomba_dead);
  });
}

static auto yellow_goomba_controller(GoombaState& goomba, LevelState& level){
  controller_base(goomba, level);
  run_walk_animation(goomba, textures::yellow_goomba_walk);

  monster_controller::die_when_stomped(goomba, level, [&]{ 
    set_dead(goomba, textures::yellow_goomba_dead);
  });
}

} //namespace mario::goomba_controller

namespace mario{

static auto run_controller(GoombaState& goomba, LevelState& level){
  using Type = GoombaState::Type;
  switch(goomba.type){
    case Type::Normal: goomba_controller::normal_goomba_controller(goomba, level); return;
    case Type::Red: goomba_controller::red_goomba_controller(goomba, level); return;
    case Type::Yellow: goomba_controller::yellow_goomba_controller(goomba, level); return;
  }
}

} //namespace mario
