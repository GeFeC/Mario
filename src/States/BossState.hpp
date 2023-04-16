#pragma once

#include "States/MonsterState.hpp"

struct BossState : MonsterState{
  int hp = 0;
  bool is_highlighted = false;

  BossState(){
    current_texture = &textures::dirt;
    is_active = true;
    can_be_stomped = false;
  }
};

struct KingGoombaState : BossState{
  static constexpr auto MaxHp = 20;
  static constexpr auto JumpPower = -20.f;

  KingGoombaState() : BossState(){
    size = glm::vec2(3.75, 4.5) * BlockBase::Size;
    hp = MaxHp;
    gravity_boost = 0.5f;
  }
};
