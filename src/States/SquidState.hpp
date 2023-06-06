#pragma once

#include "States/MonsterState.hpp"

struct SquidState : MonsterState{
  static constexpr auto AttackDistance = 3.f * BlockBase::Size;

  enum class Direction{
    Left = -1, Right = 1, None
  } direction = Direction::None;
  
  float attack_distance = 0.f;
  float attack_cooldown = 1.f;

  static auto make(const glm::vec2& position){
    auto squid = SquidState();
    squid.current_texture = &textures::squid1;
    squid.size = glm::vec2(1.f, 1.5f) * BlockBase::Size;
    squid.position = position * BlockBase::Size;
    squid.can_be_stomped = false;
    squid.reward_for_killing = 200.f;

    return squid;
  }
};
