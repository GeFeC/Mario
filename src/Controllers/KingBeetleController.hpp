#pragma once

#include "Controllers/EntityController.hpp"
#include "Controllers/MonsterController.hpp"
#include "Controllers/ShellMonsterController.hpp"
#include "Controllers/BossController.hpp"
#include "Controllers/FireballController.hpp"

#include "States/BossState.hpp"
#include "Util/Direction.hpp"
#include "res/textures.hpp"

namespace mario{

static auto run_controller(KingBeetleState& boss, LevelState& level){
  if (boss.hp > 0){
    entity_controller::run_movement_animation(boss, textures::beetle_walk);
  }

  monster_controller::endure_fireball(boss, level.player);
  entity_controller::handle_gravity(boss, level);
  boss_controller::run(boss, level);
  boss_controller::handle_walking(boss, level);

  if (boss.hp == 0) {
    return;
  }

  //Boss fireballs:
  static constexpr auto FireballSpeed = 10.f;
  static constexpr auto FireballGravity = -20.f;

  using util::Direction;
  if (boss.fireballs[0].is_on_ground && !boss.did_fireballs_seperate){
    boss.did_fireballs_seperate = true;
    boss.fireballs[0].gravity = boss.fireballs[1].gravity = FireballGravity * 2 / 3;
    boss.fireballs[0].is_on_ground = boss.fireballs[1].is_on_ground = false;

    boss.fireballs[0].set_direction(Direction::left(), FireballSpeed);
    boss.fireballs[1].set_direction(Direction::right(), FireballSpeed);
  }

  for (auto& fireball : boss.fireballs){
    fireball.explosion.run();

    entity_controller::handle_gravity(fireball, level);
    entity_controller::kill_player_on_touch(fireball, level);
    entity_controller::handle_movement(fireball, level);

    if (fireball.is_active && fireball.is_on_ground && boss.did_fireballs_seperate){
      fireball_controller::reset(fireball);
    }

    if (fireball.is_active && fireball.acceleration.left == 0.f && fireball.acceleration.right == 0.f) {
      fireball_controller::reset(fireball);
    }

    if (boss.fireball_cooldown <= 0.f) {
      const auto direction = boss.direction.as_binary();

      const auto fireball_position = boss.position + glm::vec2(direction * boss.size.x, boss.size.y / 2.f);
      fireball.shoot(fireball_position, boss.direction, FireballSpeed);
      fireball.gravity = FireballGravity;
			sounds::sounds[sounds::Fireball].play();
    }
  }

  if (boss.fireball_cooldown <= 0.f){
    boss.fireball_cooldown = KingBeetleState::FireballCooldown;
  }

  boss.fireball_cooldown -= window::delta_time;

  if (boss.fireball_cooldown <= 0.f){
    boss.did_fireballs_seperate = false;
  }

  //Minion:
  static constexpr auto MinionYToRespawn = 80.f * BlockBase::Size;
  using RespawnSide = KingBeetleState::MinionRespawnSide;
  auto& minion = level.game_objects.get<BeetleState>(0);

  //Minion Spawning
  if (minion.position.y >= MinionYToRespawn){
    minion.gravity = 0.f;
    minion.in_shell = false;
    minion.walk_speed = BeetleState::DefaultWalkSpeed;
    minion.should_collide = true;
    minion.was_hit = false;
    minion.vertical_flip = util::Flip::no_flip();

    switch(boss.minion_respawn_side){
      case RespawnSide::Left: 
        minion.position = glm::vec2(2.5f, 3.f) * BlockBase::Size;
        minion.set_direction(Direction::right());
        boss.minion_respawn_side = RespawnSide::Right;
        break;

      case RespawnSide::Right: 
        minion.position = glm::vec2(16.5f, 3.f) * BlockBase::Size;
        minion.set_direction(Direction::left());
        boss.minion_respawn_side = RespawnSide::Left;
        break;
    };
  }
  
  //Minion Attacking
  const auto boss_was_hit_by_shell = shell_monster_controller::did_hit_monster_with_shell(minion, boss);
  if (boss.can_be_hit_by_shell && boss_was_hit_by_shell){
    boss.can_be_hit_by_shell = false;
    boss_controller::take_damage(boss);
    monster_controller::bounce_out(minion);
    minion.was_hit = true;
		sounds::sounds[sounds::Shot].play();
  }

  if (!boss_was_hit_by_shell){
    boss.can_be_hit_by_shell = true;
  }
}

} //namespace mario
