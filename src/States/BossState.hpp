#pragma once

#include "States/FlameState.hpp"
#include "States/LakitoState.hpp"
#include "States/MonsterState.hpp"
#include "States/FireballState.hpp"
#include "States/BlackPlantState.hpp"
#include "Util/LoopedCounter.hpp"
#include "Util/Random.hpp"

namespace mario{

/*
 *  Dummy structure created only for checking if entity is a boss at copile time. Every boss inherits from it 
 * */
struct BossBase{};

template<typename EntityBase = MonsterState>
struct BossState : EntityBase, BossBase{
  static constexpr auto WalkSpeed = 7.f;
  static constexpr auto MaxHp = 20;

  float blink_cooldown = 0.f;
  int hp = 0;
  bool is_highlighted = false;

  BossState(){
    EntityBase::current_texture = &textures::dirt;
    EntityBase::is_active = true;
    EntityBase::can_be_stomped = false;
    hp = MaxHp;
  }
};

struct KingGoombaState : BossState<>{
  static constexpr auto Size = glm::vec2(3.00, 3.6) * BlockBase::Size;
  static constexpr auto JumpPower = -20.f;

  KingGoombaState() : BossState(){
    size = Size;
    gravity_boost = 0.5f;
    walk_speed = 6.f;
  }
};

struct KingKoopaState : BossState<>{
  static constexpr auto Size = glm::vec2(2.5f, 3.75f) * BlockBase::Size;
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
    walk_speed = WalkSpeed;
    texture_flip = util::Flip::flip();
  }
};

struct KingBeetleState : BossState<>{
  static constexpr auto MaxHp = 10;
  static constexpr auto Size = glm::vec2(2.5f) * BlockBase::Size;
  static constexpr auto WalkSpeed = 4.f;
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
    texture_flip = util::Flip::flip();
    hp = MaxHp;
  } 
};

struct KingCheepState : BossState<>{
  static constexpr auto Size = glm::vec2(2.5f, 2.5f) * BlockBase::Size;
  static constexpr auto MaxHp = 20;

  float timer = 0.f;
  float rotation = 0.f;

  KingCheepState() : BossState(){
    size = Size;
    walk_speed = 6.f;
    hp = MaxHp;
  }
};

struct KingPlantState : BossState<BlackPlantState>{
  static constexpr auto Size = glm::vec2(2.5f, 2.5f * 11.f / 8.f) * BlockBase::Size;
  static constexpr auto MaxHp = 20;

  KingPlantState() : BossState(){
    ((*this) | util::as<BlackPlantState&>) = BlackPlantState::make({ 0, 0 });

    hp = MaxHp;
    size = Size;

    shot_boost = 1.5f;
    cooldown_duration = 0.5f;
    speed = 150.f;
  }
};

struct KingLakitoState : BossState<LakitoState>{
  static constexpr auto InitialPosition = glm::vec2(8.5f, 7.f) * BlockBase::Size;
  static constexpr auto Size = glm::vec2(2.5f, 2.5f * 11.f / 8.f) * BlockBase::Size;
  static constexpr auto MaxHp = 10;

  float timer = 0.f;

  std::array<ThrownSpikeState, 2> extra_thrown_spikes;

  KingLakitoState() : BossState(){
    ((*this) | util::as<LakitoState&>) = LakitoState::make({ 0, 0 });

    throw_counter = util::InfiniteCounter(10.f, 3.f);
    can_be_stomped = false;
    is_active = true;
    size = Size;
    position = InitialPosition;
    hp = MaxHp;
    should_collide = true;
    flip_gravity();

    for (auto& spike : extra_thrown_spikes){
      spike = ThrownSpikeState::make();
    }
  }
};

struct KingBowserState : BossState<>{
  static constexpr auto Size = glm::vec2(3.f) * BlockBase::Size;
  static constexpr auto MaxHp = 20;
	static constexpr auto NoTarget = glm::vec2(-1.f);
	static constexpr auto InitialPosition = glm::vec2(8.5f, 5.f) * BlockBase::Size;
	static constexpr auto AttackIndicatorSize = glm::vec2(BlockBase::Size * 2.f);

	enum class AttackState{
		Flame,
		Charge
	} attack_state = AttackState::Flame;

	util::LoopedCounter charge_counter = util::LoopedCounter(12.f, 10.f, 1);
	glm::vec2 charge_target_position = NoTarget;
	glm::vec2 charge_start_position = InitialPosition;
	glm::vec2 attack_indicator_pos;
	float attack_indicator_opacity = 0.f;
	int attacks_count = 0;

	float velocity = 0.f;
	glm::vec2 temp_direction;

	util::Generator<FlameState> flames_generator;
	util::LoopedCounter breathe_counter = util::LoopedCounter(1.f, 10.f, 1);
	float rotation = 0.f;

  KingBowserState() : BossState(){
		position = InitialPosition;
		current_texture = &textures::king_bowser_walk[0];
    can_be_stomped = false;
    is_active = true;
    size = Size;
    hp = MaxHp;
    should_collide = true;
		texture_flip = util::Flip::flip();
  }
};

} //namespace mario
