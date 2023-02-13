#pragma once

#include "States/KoopaState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/ShellMonsterController.hpp"
#include "res/textures.hpp"

static auto flying_koopa_controller(
    FlyingKoopaState& koopa, 
    PlayerState& player, 
    LevelState& level, 
    const std::array<Texture, 2>& walk_frames_with_wings,
    const std::array<Texture, 2>& walk_frames_without_wings
){
  if (koopa.has_wings && koopa.is_on_ground){
    static constexpr auto FlyingKoopaJumpHeight = -15;

    koopa.gravity = FlyingKoopaJumpHeight;
    koopa.is_on_ground = false;
  }

  if (koopa.has_wings){
    shell_monster_controller(koopa, level, walk_frames_with_wings);
  }
  else{
    shell_monster_controller(koopa, level, walk_frames_without_wings);
  }

  //Interaction with blocks
  entity_die_when_on_bouncing_block(koopa, level);

  //Interaction with player
  entity_die_when_hit_by_fireball(koopa, player, level.stats);
  entity_become_active_when_seen(koopa, player);

  if (player_stomp_on_entity(player, koopa) && koopa.has_wings){
    koopa.has_wings = false;
    player.gravity = PlayerState::BouncePower;
    koopa.gravity = 0;

    koopa.spawn_points(player.mobs_killed_in_row);
    level.stats.score += koopa.reward_for_killing * player.mobs_killed_in_row;
    return;
  }

  if (koopa.has_wings) {
    entity_kill_player_on_touch(koopa, player);
    return;
  }

  entity_handle_shell(
    koopa,
    player,
    level,
    textures::green_koopa_dead
  );
}

static auto green_flying_koopa_controller(FlyingKoopaState& koopa, PlayerState& player, LevelState& level){
  flying_koopa_controller(
    koopa,
    player,
    level,
    textures::green_flying_koopa_walk,
    textures::green_koopa_walk
  );
}