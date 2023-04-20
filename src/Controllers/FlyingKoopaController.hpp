#pragma once

#include "Controllers/KoopaController.hpp"
#include "States/FlyingKoopaState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/ShellMonsterController.hpp"
#include "Window.hpp"

static auto flying_koopa_controller(
    FlyingKoopaState& koopa, 
    LevelState& level, 
    const std::array<Texture, 2>& walk_frames_with_wings,
    const std::array<Texture, 2>& walk_frames_without_wings,
    float& timer
){
  //Motion
  if (koopa.has_wings && koopa.should_collide){
    const auto previous_x = koopa.position.x;

    auto distance = glm::sqrt(
      glm::pow(koopa.movement_axis.x, 2) +
      glm::pow(koopa.movement_axis.y, 2)
    );

    static constexpr auto FlightSpeedMultiplier = 2.f;

    const auto sin = glm::sin(timer / distance * koopa.walk_speed * FlightSpeedMultiplier) | util::as<float>;
    koopa.position = koopa.initial_position + koopa.movement_axis * sin * BlockBase::Size;

    //Turning around
    if (previous_x - koopa.position.x >= 0.f){
      koopa.direction = EntityState::DirectionLeft;
    }
    else{
      koopa.direction = EntityState::DirectionRight;
    }

    entity_run_movement_animation(koopa, walk_frames_with_wings);
  }
  else{
    koopa.set_direction(koopa.direction);
    shell_monster_controller(koopa, level, walk_frames_without_wings);
  }

  //Interaction with player
  auto& player = level.player;

  entity_die_when_hit_by_fireball(koopa, level);
  entity_become_active_when_seen(koopa, level);

  if (koopa.has_wings){
    auto koopa_hitbox = shell_monster_get_hitbox(koopa);
    entity_die_when_stomped(koopa, level, [&]{
      koopa.has_wings = false;
      koopa.gravity = 0;
    });

    entity_kill_player_on_touch(koopa_hitbox, player);
  }
}

static auto green_flying_koopa_controller(FlyingKoopaState& koopa, LevelState& level){
  flying_koopa_controller(
    koopa,
    level,
    textures::green_flying_koopa_walk,
    textures::green_koopa_walk,
    LevelState::timer
  );

  entity_handle_shell(
    koopa,
    level,
    textures::green_koopa_dead
  );
}

static auto red_flying_koopa_controller(FlyingKoopaState& koopa, LevelState& level){
  flying_koopa_controller(
    koopa,
    level,
    textures::red_flying_koopa_walk,
    textures::red_koopa_walk,
    LevelState::timer
  );

  entity_handle_shell(
    koopa,
    level,
    textures::red_koopa_dead
  );
}

static auto purple_flying_koopa_controller(FlyingKoopaState& koopa, LevelState& level){
  purple_koopa_movement_controller(koopa, level);

  static auto previous_walk_speed = 0;

  auto& timer = level.purple_flying_koopa_timer;
  if (koopa.has_wings){
    if (previous_walk_speed != koopa.walk_speed) {
      timer = timer * previous_walk_speed / koopa.walk_speed;
    }

    previous_walk_speed = koopa.walk_speed;
  }

  flying_koopa_controller(
    koopa,
    level,
    textures::purple_flying_koopa_walk,
    textures::purple_koopa_walk,
    level.purple_flying_koopa_timer
  );

  entity_handle_shell(
    koopa,
    level,
    textures::purple_koopa_dead
  );
}
