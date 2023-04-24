#pragma once

#include "States/MonsterState.hpp"

struct BossState : MonsterState{
  static constexpr auto WalkSpeed = 7.f;

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
    walk_speed = 7.f;
  }
};

struct KingKoopaState : BossState{
  static constexpr auto MaxHp = 20;
  static constexpr auto Size = glm::vec2(3.f, 4.5f) * BlockBase::Size;
  static constexpr auto ShellSpeed = 20.f;

  static constexpr auto SpeedUpDelay = 0.5f;
  static constexpr auto WithdrawDuration = 2.f;

  static auto get_withdraw_delay() -> float{
    return util::random_value(2.f, 3.f);
  };

  float withdraw_duration = WithdrawDuration;
  float withdraw_delay = get_withdraw_delay();
  float speedup_delay = SpeedUpDelay;
  bool in_shell = false;
  bool should_reset = false;

  KingKoopaState() : BossState(){
    size = Size;
    hp = MaxHp;
    walk_speed = WalkSpeed;
    texture_flip = Drawable::Flip::UseFlip;
  }
};
