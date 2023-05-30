#pragma once

#include "States/MonsterState.hpp"
#include "States/FireballState.hpp"

struct BossState : MonsterState{
  static constexpr auto WalkSpeed = 7.f;
  static constexpr auto MaxHp = 20;

  float blink_cooldown = 0.f;
  int hp = 0;
  bool is_highlighted = false;

  BossState(){
    current_texture = &textures::dirt;
    is_active = true;
    can_be_stomped = false;
    hp = MaxHp;
  }
};

struct KingGoombaState : BossState{
  static constexpr auto Size = glm::vec2(3.75, 4.5) * BlockBase::Size;
  static constexpr auto JumpPower = -20.f;

  KingGoombaState() : BossState(){
    size = Size;
    gravity_boost = 0.5f;
    walk_speed = 6.f;
  }
};

struct KingKoopaState : BossState{
  static constexpr auto Size = glm::vec2(3.f, 4.5f) * BlockBase::Size;
  static constexpr auto ShellSpeed = 15.f;

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
    walk_speed = WalkSpeed;
    texture_flip = Drawable::Flip::UseFlip;
  }
};

struct KingBeetleState : BossState{
  static constexpr auto Size = glm::vec2(3.f) * BlockBase::Size;
  static constexpr auto WalkSpeed = 4.f;
  static constexpr auto MaxHp = 10.f;
  static constexpr auto FireballCooldown = 4.f;

  std::array<FireballState, 2> fireballs;
  float fireball_cooldown = FireballCooldown;
  bool did_fireballs_seperate = false;
  bool can_be_hit_by_shell = true;

  enum class MinionRespawnSide{
    Left, Right
  } minion_respawn_side = MinionRespawnSide::Right;

  KingBeetleState() : BossState() {
    size = Size;
    walk_speed = WalkSpeed;
    texture_flip = Drawable::Flip::UseFlip;
    hp = MaxHp;
  } 
};
