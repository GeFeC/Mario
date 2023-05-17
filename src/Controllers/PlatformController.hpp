#pragma once

#include "PolyControllers.hpp"

#include "States/EntityState.hpp"
#include "States/FlyingKoopaState.hpp"
#include "States/JumpingKoopaState.hpp"
#include "States/PlatformState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/EntityController.hpp"
#include "Util/Util.hpp"
#include "Window.hpp"
#include "config.hpp"
#include <iterator>

static auto platform_collision_controller(
    PlatformState& platform, 
    LevelState& level, 
    const glm::vec2& previous_pos
  ){
  const auto is_entity_on_platform = [&](const EntityState& entity){
    if (entity.gravity < 0) return false;

    if (entity.position.x - platform.position.x == util::in_range(-entity.size.x, platform.size().x)){
      return entity.position.y + entity.size.y - platform.position.y == util::in_range(-15.f, 15.f);
    } 

    return false;
  };

  auto& player = level.player;
  if (is_entity_on_platform(player)){
    platform.is_active = true;

    player.position += platform.position - previous_pos;
  }

  level.game_objects.for_each_derived<EntityState>([&](auto& entity){
    if (is_entity_on_platform(entity)){
      entity.is_on_platform = true;

      entity.position += platform.position - previous_pos;

      if constexpr (std::is_convertible_v<decltype(entity), ShellMonsterState>){
        if (entity.in_shell) return;
      }

      if constexpr (std::is_convertible_v<decltype(entity), FlyingKoopaState>){
        if (entity.has_wings) return;
      }

      if (entity.fall_from_edge) return;

      if (entity.position.x < platform.position.x - MonsterState::EdgeDetectionOffset){
        entity.turn_around();
        entity.position.x = platform.position.x - MonsterState::EdgeDetectionOffset;
      }

      const auto platform_right_edge_pos = platform.position.x + platform.size().x;
      if (entity.position.x + entity.size.x > platform_right_edge_pos + MonsterState::EdgeDetectionOffset){
        entity.turn_around();
        entity.position.x = platform_right_edge_pos - entity.size.x + MonsterState::EdgeDetectionOffset;
      }

      entity_turn_around(entity);
    }
  });
}

template<>
struct Controller<PlatformState>{
  static auto run(PlatformState& platform, LevelState& level){
    static constexpr auto PlatformSpeed = PlatformState::Speed;
    const auto sin = (glm::sin(platform.timer / platform.move_distance() * PlatformSpeed) + 1) / 2.f;

    const auto previous_pos = platform.position;
    platform.position = platform.initial_position + (sin | util::as<float>) * platform.transport_distance;

    platform_collision_controller(platform, level, previous_pos);
    if (!platform.is_active){
      //Make glm::sin return -1 
      platform.timer = glm::radians(-90.f * platform.move_distance() / PlatformSpeed);
    }

    auto& player = level.player;

    if (!platform.is_active) return;

    platform.timer += window::delta_time;
  }
};

template<>
struct Controller<LoopedPlatformState>{
  static auto run(LoopedPlatformState& platform, LevelState& level){
    const auto previous_pos = platform.position;
    static constexpr auto LoopedPlatformSpeed = 5.f; 
    platform.position += platform.transport_distance * LoopedPlatformSpeed * window::delta_time;

    platform_collision_controller(platform, level, previous_pos);

    if (platform.position.y > config::FrameBufferSize.y) platform.position.y = -PlatformState::ElementSize;
    if (platform.position.y < -PlatformState::ElementSize) platform.position.y = config::FrameBufferSize.y;
  }
};
