#pragma once

#include "Controllers/KoopaController.hpp"
#include "States/FlyingKoopaState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/ShellMonsterController.hpp"
#include "Window.hpp"

namespace mario::flying_koopa_controller{

static auto run_controller_base(
    FlyingKoopaState& koopa, 
    LevelState& level, 
    const std::array<renderer::Texture, 2>& walk_frames_with_wings,
    const std::array<renderer::Texture, 2>& walk_frames_without_wings,
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
      koopa.direction = util::Direction::left();
    }
    else{
      koopa.direction = util::Direction::right();
    }

    monster_controller::run_movement_animation(koopa, walk_frames_with_wings);
  }
  else{
    koopa.set_direction(koopa.direction);
    shell_monster_controller::run(koopa, level, walk_frames_without_wings);
  }

  //Interaction with player
  monster_controller::die_when_hit_by_fireball(koopa, level);
  monster_controller::become_active_when_seen(koopa, level);

  if (koopa.has_wings){
    auto koopa_hitbox = shell_monster_controller::get_hitbox(koopa);
    monster_controller::die_when_stomped(koopa, level, [&]{
      koopa.has_wings = false;
      koopa.gravity = 0;
    });

    entity_controller::kill_player_on_touch(koopa_hitbox, level);
  }
}

static auto run_green_koopa_controller(FlyingKoopaState& koopa, LevelState& level){
  run_controller_base(
    koopa,
    level,
    textures::green_flying_koopa_walk,
    textures::green_koopa_walk,
    LevelState::timer
  );

  shell_monster_controller::handle_shell(
    koopa,
    level,
    textures::green_koopa_dead
  );
}

static auto run_red_koopa_controller(FlyingKoopaState& koopa, LevelState& level){
  run_controller_base(
    koopa,
    level,
    textures::red_flying_koopa_walk,
    textures::red_koopa_walk,
    LevelState::timer
  );

  shell_monster_controller::handle_shell(
    koopa,
    level,
    textures::red_koopa_dead
  );
}

static auto run_purple_koopa_controller(FlyingKoopaState& koopa, LevelState& level){
  koopa_controller::handle_purple_koopa_speedup(koopa, level);

  static auto previous_walk_speed = 0.f;

  auto& timer = level.purple_flying_koopa_timer;
  if (koopa.has_wings && !koopa.was_hit){
    if (previous_walk_speed != koopa.walk_speed) {
      timer = timer * previous_walk_speed / koopa.walk_speed;
    }

    previous_walk_speed = koopa.walk_speed;
  }

  run_controller_base(
    koopa,
    level,
    textures::purple_flying_koopa_walk,
    textures::purple_koopa_walk,
    level.purple_flying_koopa_timer
  );

  shell_monster_controller::handle_shell(
    koopa,
    level,
    textures::purple_koopa_dead
  );
}

} //namespace mario::flying_koopa_controller

namespace mario{

static auto run_controller(FlyingKoopaState& koopa, LevelState& level){
  using Type = KoopaState::Type;
  switch(koopa.type){
    case Type::Green: flying_koopa_controller::run_green_koopa_controller(koopa, level); return;
    case Type::Red: flying_koopa_controller::run_red_koopa_controller(koopa, level); return;
    case Type::Purple: flying_koopa_controller::run_purple_koopa_controller(koopa, level); return;
  }
}

} //namespace mario
