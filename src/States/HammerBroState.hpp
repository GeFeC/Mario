#pragma once

#include "States/LoopedCounter.hpp"
#include "States/MonsterState.hpp"
#include "States/HammerState.hpp"
#include "Util/Util.hpp"
#include "Util/Generator.hpp"

struct HammerBroState : MonsterState{
  static constexpr auto MaxWalkDistance = config::BlockSize * 1.f;
  static constexpr auto JumpPower = -30.f;
  static constexpr auto WeakJumpPower = -20.f;
  static constexpr auto HammerThrowForce = -25.f;

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
  int hammers_spawned = 0;
  bool started_jumping = false;

  HammerBroState() : throw_counter(config::BigValue, 3.f, 1) {}

  static auto new_jump_delay(){
    return util::random_value(3, 5);
  }

  static auto new_throw_delay(){
    return util::random_value(1, 2);
  }

  static auto make(const glm::vec2& position){
    auto bro = HammerBroState();
    bro.position = position * config::BlockSize;
    bro.initial_x = bro.position.x;
    bro.size = glm::vec2(1, 2) * config::BlockSize;
    bro.walk_speed = 3.f;
    bro.current_texture = &textures::hammerbro_walk[0];
    bro.set_direction(DirectionLeft);
    bro.jump_delay = new_jump_delay();
    bro.throw_delay = new_throw_delay();
    bro.is_active = true;
    bro.reward_for_killing = 500;

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
