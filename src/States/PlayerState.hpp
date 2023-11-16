#pragma once

#include "States/EntityState.hpp"
#include "States/FireballState.hpp"
#include "Util/LoopedCounter.hpp"
#include "config.hpp"

#include <unordered_map>
#include <array>

namespace mario{

struct PlayerState : EntityState{
  static constexpr auto FireballSpeed = 18.f;

  static constexpr auto MaxSpeedWithoutSprint = 7.f;
  static constexpr auto MaxSpeedWithSprint = 10.f;
  static constexpr auto MaxSpeedWhenSquating = 1.5f;

  static constexpr auto MaxGravityWhenSwimming = 7.f;

  static constexpr auto BouncePower = -23.f;
  static constexpr auto DeathBouncePower = -23.f;
  static constexpr auto JumpPower = -30.f;
  static constexpr auto SwimPower = -18.f;

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

  util::LoopedCounter swim_counter = util::LoopedCounter(2.f, 10.f, 1);
  util::LoopedCounter growth_counter = util::LoopedCounter(3.f, 10.f, 3);
  util::LoopedCounter transformation_counter = util::LoopedCounter(2.f, 10.f, 3);
  std::array<FireballState, 2> fireballs;

  int mobs_killed_in_row = 1;
  float current_walk_animation_frame = 0.f;

  float max_speed = 0.f;
  float invincibility_delay = 0.f;
  float grow_state = 0.f;
  float slip = 1.f;

  bool swim_cooldown = false;
  bool jump_cooldown = false;
  bool can_move = true;
  bool is_growing_up = false;
  bool is_squating = false;
  bool must_squat = false;
  bool is_shrinking = false;
  bool is_changing_to_fire = false;

  PlayerState(){
    death_delay = 0.5f;
    size = glm::vec2(BlockBase::Size);
    is_active = true;
    current_texture = &textures::small_mario;

    swim_counter.disable();
  }

  auto set_form_to_fire(){
    growth = PlayerState::Growth::Big;
    form = PlayerState::Form::Fire;
    position.x = 2 * BlockBase::Size;
  }

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

  auto swim_texture(int swim_frame) const{
    if (form == Form::Normal){
      switch(growth){
        case Growth::Small: return &textures::small_mario_swim[swim_frame];
        case Growth::Big: return &textures::big_mario_swim[swim_frame];
        default: return &textures::small_mario;
      }
    }

    if (form == Form::Fire) return &textures::fire_big_mario_swim[swim_frame];
    if (form == Form::Black) return &textures::black_big_mario_swim[swim_frame];

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
        default: break;
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

} //namespace mario
