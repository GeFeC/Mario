#pragma once

#include "Util/LoopedCounter.hpp"
#include "States/MonsterState.hpp"
#include "States/HammerState.hpp"
#include "Util/Util.hpp"
#include "Util/Generator.hpp"
#include "res/textures.hpp"

namespace mario{

struct HammerBroState : MonsterState{
  static constexpr auto MaxWalkDistance = BlockBase::Size;
  static constexpr auto JumpPower = -30.f;
  static constexpr auto WeakJumpPower = -20.f;
  static constexpr auto HammerThrowForce = -25.f;

  enum class Type{
    Green,
    Red
  } type;

  enum class JumpState{
    Up,
    Down
  } jump_state = JumpState::Up;

  util::Generator<HammerState> hammer_generator;
  util::LoopedCounter throw_counter;
  Direction walk_direction = DirectionLeft;

  float jump_delay;
  float throw_delay;
  float initial_x;

  int hammers_count = 0;
  int hammers_spawned = 0;
  bool started_jumping = false;

  HammerBroState() : throw_counter(util::BigValue, 3.f, 1) {}

  static auto new_jump_delay(){
    return util::random_value(3, 5);
  }

  static auto new_throw_delay(){
    return util::random_value(1, 2);
  }

  static auto make(const glm::vec2& position){
    auto bro = HammerBroState();
    bro.position = (position + glm::vec2(0.f, -1.f)) * BlockBase::Size;
    bro.initial_x = bro.position.x;
    bro.size = glm::vec2(1, 2) * BlockBase::Size;
    bro.walk_speed = 3.f;
    bro.current_texture = &textures::hammerbro_walk[0];
    bro.set_direction(DirectionLeft);
    bro.jump_delay = new_jump_delay();
    bro.throw_delay = new_throw_delay();
    bro.reward_for_killing = 500;
    bro.hammers_count = 3;
    bro.type = Type::Green;

    return bro;
  }

  static auto make_red(const glm::vec2& position){
    auto bro = make(position);
    bro.throw_counter.step = 15.f;
    bro.hammers_count = 15;
    bro.type = Type::Red;
    bro.current_texture = &textures::red_hammerbro_walk[0];

    return bro;
  }

  auto spawn_hammer(){
    hammer_generator.make_item_if_needed();
    auto& hammer = hammer_generator.item();
    hammer.is_active = true;
    hammer.is_visible = true;
    hammer.position = position;
    hammer.set_direction(direction);
    hammer.gravity = HammerThrowForce;
  }
};

} //namespace mario
