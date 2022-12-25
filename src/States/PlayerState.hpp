#pragma once

#include "States/EntityState.hpp"
#include "States/LoopedCounter.hpp"
#include "States/FireballState.hpp"
#include "config.hpp"

#include <unordered_map>
#include <array>

struct PlayerState : EntityState{
  enum class Growth{
    Small,
    Medium,
    Big
  } growth = Growth::Small;

  enum class Form{
    Normal,
    Fire,
    Black
  } form = Form::Normal;

  LoopedCounter growth_counter;
  LoopedCounter transformation_counter;
  std::array<FireballState, 2> fireballs;

  int mobs_killed_in_row = 1;

  float invincibility_delay = 0.f;
  float grow_state = 0.f;
  
  bool is_growing_up = false;
  bool is_squating = false;
  bool must_squat = false;
  bool is_shrinking = false;
  bool is_changing_to_fire = false;

  PlayerState() 
  : growth_counter(3.f, 10.f, 3), transformation_counter(2.f, 10.f, 3) {}

  auto default_texture() const{
    if (form == Form::Normal){
      switch(growth){
        case Growth::Small: return &textures::small_mario;
        case Growth::Medium: return &textures::medium_mario;
        case Growth::Big: return &textures::big_mario;
      }
    }

    if (form == Form::Fire) return &textures::fire_big_mario;
    if (form == Form::Black) return &textures::black_big_mario;

    return &textures::dirt;
  }

  auto walk_texture(int walk_frame) const{
    if (form == Form::Normal){
      switch(growth){
        case Growth::Small: return &textures::small_mario_walk[walk_frame];
        case Growth::Big: return &textures::big_mario_walk[walk_frame];
        default: return &textures::small_mario;
      }
    }

    if (form == Form::Fire) return &textures::fire_big_mario_walk[walk_frame];
    if (form == Form::Black) return &textures::black_big_mario_walk[walk_frame];

    return &textures::dirt;
  }

  auto turn_texture() const{
    if (form == Form::Normal){
      switch(growth){
        case Growth::Small: return &textures::small_mario_turning;
        case Growth::Big: return &textures::big_mario_turning;
        default: return &textures::small_mario;
      }
    }

    if (form == Form::Fire) return &textures::fire_big_mario_turning;
    if (form == Form::Black) return &textures::black_big_mario_turning;

    return &textures::dirt;
  }

  auto jump_texture() const{
    if (form == Form::Normal){
      switch(growth){
        case Growth::Small: return &textures::small_mario_jumping;
        case Growth::Big: return &textures::big_mario_jumping;
      }
    }

    if (form == Form::Fire) return &textures::fire_big_mario_jumping;
    if (form == Form::Black) return &textures::black_big_mario_jumping;

    return &textures::dirt;
  }

  auto squat_texture() const{
    if (form == Form::Normal) return &textures::big_mario_squating;
    if (form == Form::Fire) return &textures::fire_big_mario_squating;
    if (form == Form::Black) return &textures::black_big_mario_squating;

    return &textures::dirt;
  }
};
