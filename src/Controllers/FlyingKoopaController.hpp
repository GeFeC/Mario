#pragma once

#include "States/FlyingKoopaState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/ShellMonsterController.hpp"

static auto flying_koopa_controller(
    FlyingKoopaState& koopa, 
    LevelState& level, 
    const std::array<Texture, 2>& walk_frames_with_wings,
    const std::array<Texture, 2>& walk_frames_without_wings
){
  //Motion
  const auto timer = FlyingKoopaState::timer - koopa.start_time;
  if (koopa.has_wings && koopa.should_collide){
    const auto previous_x = koopa.position.x;

    koopa.position = glm::vec2(
      koopa.initial_position.x + glm::sin(timer) * config::BlockSize * koopa.movement_axis.x,
      koopa.initial_position.y + glm::sin(timer) * config::BlockSize * koopa.movement_axis.y
    );

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

  entity_die_when_hit_by_fireball(koopa, player, level.stats);
  entity_become_active_when_seen(koopa, player);

  if (player_stomp_on_entity(player, koopa) && koopa.has_wings && !player.is_dead){
    koopa.has_wings = false;
    player.gravity = PlayerState::BouncePower;
    koopa.gravity = 0;

    koopa.spawn_points(player.mobs_killed_in_row);
    level.stats.score += koopa.reward_for_killing * player.mobs_killed_in_row;
    return;
  }

  if (koopa.has_wings) {
    entity_kill_player_on_touch(koopa, player);
  }
}

static auto green_flying_koopa_controller(FlyingKoopaState& koopa, LevelState& level){
  flying_koopa_controller(
    koopa,
    level,
    textures::green_flying_koopa_walk,
    textures::green_koopa_walk
  );

  entity_handle_shell(
    koopa,
    level.player,
    level,
    textures::green_koopa_dead
  );
}

static auto red_flying_koopa_controller(FlyingKoopaState& koopa, LevelState& level){
  flying_koopa_controller(
    koopa,
    level,
    textures::red_flying_koopa_walk,
    textures::red_koopa_walk
  );

  entity_handle_shell(
    koopa,
    level.player,
    level,
    textures::red_koopa_dead
  );
}
