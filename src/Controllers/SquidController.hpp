#pragma once

#include "Controllers/EntityController.hpp"
#include "Controllers/MonsterController.hpp"
#include "Controllers/PointsParticlesController.hpp"

#include "States/BlockState.hpp"
#include "States/SquidState.hpp"
#include "Window.hpp"
#include "config.hpp"

namespace mario{

static auto run_controller(SquidState& squid, LevelState& level){
  monster_controller::become_active_when_seen(squid, level);    
  monster_controller::die_when_hit_by_fireball(squid, level);
  monster_controller::kill_player_on_touch(squid, level);
  monster_controller::handle_points_particles(squid);

  if (squid.was_hit){
    entity_controller::handle_gravity(squid, level);
    return;
  }

  if (squid.position.y / BlockBase::Size > level.max_size().y) squid.is_active = false;
  if (!squid.is_active) return;
  squid.position.y = std::max(squid.position.y, BlockBase::Size);

  const auto& player = level.player;

  static constexpr auto WalkSpeed = 200.f;
  static constexpr auto AttackDistance = SquidState::AttackDistance;
  const auto is_attacking = squid.attack_distance > 0.f;

  if (squid.position.y >= player.position.y && squid.attack_cooldown <= 0.f && !is_attacking){
    squid.attack_distance = AttackDistance;
  }

  //Movement, when not attacking:
  if (!is_attacking || squid.attack_cooldown > 0.f){
    squid.position.y += window::delta_time * WalkSpeed;
  }
  else{
    using Direction = SquidState::Direction;

    if (squid.direction == Direction::None){
      squid.direction = player.position.x > squid.position.x
        ? Direction::Right 
        : Direction::Left;

      const auto is_far_from_player 
        = player.position.x - squid.position.x > config::FrameBufferSize.x
        || player.position.y - squid.position.y > config::FrameBufferSize.y;

      if (is_far_from_player){
        squid.direction = Direction::Left;
      }
    }

    static constexpr auto AttackSpeed = 800.f;

    const auto attack_direction = squid.direction | util::as<int>;
    const auto attack_speed = AttackSpeed * window::delta_time;
    const auto translation = glm::vec2(attack_direction, -1) * attack_speed;

    squid.position += translation;
    squid.attack_distance -= attack_speed;;

    if (squid.attack_distance <= 0.f){
      squid.attack_cooldown = 1.f;
      squid.direction = Direction::None;
    }
  }

  const auto distance_from_player = squid.position.y - player.position.y;
  if (squid.attack_cooldown < 0.2f && !is_attacking && distance_from_player > -0.2f * BlockBase::Size){
    squid.current_texture = &textures::squid2;
    squid.size.y = BlockBase::Size;
  }
  else{
    squid.current_texture = &textures::squid1;
    squid.size.y = BlockBase::Size * 1.5f;
  }

  squid.attack_cooldown -= window::delta_time;
}

} //namespace mario
