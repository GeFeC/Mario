#pragma once

#include "PolyControllers.hpp"

#include "Controllers/MonsterController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "States/EntityState.hpp"
#include "States/LevelState.hpp"
#include "res/textures.hpp"
#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

static auto goomba_set_dead(GoombaState& goomba, const Texture& dead_texture){
  goomba.is_dead = true;
  goomba.current_texture = &dead_texture;
}

static auto goomba_run_walk_animation(GoombaState& goomba, const std::array<Texture, 2>& walk_frames){
  if (goomba.is_dead) return;
  monster_run_movement_animation(goomba, walk_frames);
}

static auto goomba_controller_base(GoombaState& goomba, LevelState& level){
  //Interactions with player
  auto& player = level.player;
  monster_kill_player_on_touch(goomba, player);
  monster_become_active_when_seen(goomba, level);
  monster_die_when_hit_by_fireball(goomba, level);

  //Interaction with blocks
  monster_die_when_on_bouncing_block(goomba, level);

  entity_gravity(goomba, level);
  entity_movement(goomba, level);
  monster_turn_around(goomba);
  monster_points_particles(goomba);

  if (goomba.is_dead){
    goomba.death_delay -= window::delta_time;

    if (goomba.death_delay <= 0.f){
      goomba.is_visible = false;
    }

    return;
  } 
}

static auto normal_goomba_controller(GoombaState& goomba, LevelState& level){
  goomba_controller_base(goomba, level);
  goomba_run_walk_animation(goomba, textures::goomba_walk);

  monster_die_when_stomped(goomba, level, [&]{ 
    goomba_set_dead(goomba, textures::goomba_dead);
  });
}

static auto red_goomba_controller(GoombaState& goomba, LevelState& level){
  goomba_controller_base(goomba, level);
  goomba_run_walk_animation(goomba, textures::red_goomba_walk);

  monster_die_when_stomped(goomba, level, [&]{ 
    goomba_set_dead(goomba, textures::red_goomba_dead);
  });
}

static auto yellow_goomba_controller(GoombaState& goomba, LevelState& level){
  goomba_controller_base(goomba, level);
  goomba_run_walk_animation(goomba, textures::yellow_goomba_walk);

  monster_die_when_stomped(goomba, level, [&]{ 
    goomba_set_dead(goomba, textures::yellow_goomba_dead);
  });
}

template<>
struct Controller<GoombaState>{
  static auto run(GoombaState& goomba, LevelState& level){
    using Type = GoombaState::Type;
    switch(goomba.type){
      case Type::Normal: normal_goomba_controller(goomba, level); return;
      case Type::Red: red_goomba_controller(goomba, level); return;
      case Type::Yellow: yellow_goomba_controller(goomba, level); return;
    }
  }
};
