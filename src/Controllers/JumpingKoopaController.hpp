#pragma once

#include "States/KoopaState.hpp"
#include "States/JumpingKoopaState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/ShellMonsterController.hpp"
#include "res/textures.hpp"

static auto jumping_koopa_controller(
    JumpingKoopaState& koopa, 
    LevelState& level, 
    const std::array<Texture, 2>& walk_frames_with_wings,
    const std::array<Texture, 2>& walk_frames_without_wings
){
  if (koopa.has_wings && koopa.is_on_ground){
    koopa.gravity = JumpingKoopaState::JumpForce;
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
  auto& player = level.player;
  entity_die_when_hit_by_fireball(koopa, player, level.stats);
  entity_become_active_when_seen(koopa, player);

  if (koopa.has_wings) {
    auto koopa_hitbox = shell_monster_get_hitbox(koopa);
    entity_die_when_stomped(koopa, player, level.stats, [&]{
      koopa.has_wings = false;
      koopa.gravity = 0;
    });

    entity_kill_player_on_touch(koopa_hitbox, player);
  }
}

static auto green_jumping_koopa_controller(JumpingKoopaState& koopa, LevelState& level){
  jumping_koopa_controller(
    koopa,
    level,
    textures::green_flying_koopa_walk,
    textures::green_koopa_walk
  );

  entity_handle_shell(
    koopa,
    level,
    textures::green_koopa_dead
  );
}

static auto red_jumping_koopa_controller(JumpingKoopaState& koopa, LevelState& level){
  if (!koopa.has_wings && !koopa.in_shell) koopa.fall_from_edge = false;

  jumping_koopa_controller(
    koopa,
    level,
    textures::red_flying_koopa_walk,
    textures::red_koopa_walk
  );

  entity_handle_shell(
    koopa,
    level,
    textures::red_koopa_dead
  );
}
