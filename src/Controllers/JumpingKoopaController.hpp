#pragma once

#include "PolyControllers.hpp"

#include "Controllers/KoopaController.hpp"
#include "States/KoopaState.hpp"
#include "States/JumpingKoopaState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/ShellMonsterController.hpp"
#include "res/textures.hpp"

static auto jumping_koopa_controller_base(
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
  monster_die_when_on_bouncing_block(koopa, level);

  //Interaction with player
  auto& player = level.player;
  monster_die_when_hit_by_fireball(koopa, level);
  monster_become_active_when_seen(koopa, level);

  if (koopa.has_wings) {
    auto koopa_hitbox = shell_monster_get_hitbox(koopa);
    monster_die_when_stomped(koopa, level, [&]{
      koopa.has_wings = false;
      koopa.gravity = 0;
    });

    monster_kill_player_on_touch(koopa_hitbox, player);
  }
}

static auto green_jumping_koopa_controller(JumpingKoopaState& koopa, LevelState& level){
  jumping_koopa_controller_base(
    koopa,
    level,
    textures::green_flying_koopa_walk,
    textures::green_koopa_walk
  );

  shell_monster_handle_shell(
    koopa,
    level,
    textures::green_koopa_dead
  );
}

static auto red_jumping_koopa_controller(JumpingKoopaState& koopa, LevelState& level){
  if (!koopa.has_wings && !koopa.in_shell) koopa.fall_from_edge = false;

  jumping_koopa_controller_base(
    koopa,
    level,
    textures::red_flying_koopa_walk,
    textures::red_koopa_walk
  );

  shell_monster_handle_shell(
    koopa,
    level,
    textures::red_koopa_dead
  );
}

static auto purple_jumping_koopa_controller(JumpingKoopaState& koopa, LevelState& level){
  if (!koopa.has_wings && !koopa.in_shell) koopa.fall_from_edge = false;

  jumping_koopa_controller_base(
    koopa,
    level,
    textures::purple_flying_koopa_walk,
    textures::purple_koopa_walk
  );

  shell_monster_handle_shell(
    koopa,
    level,
    textures::purple_koopa_dead
  );

  purple_koopa_movement_controller(koopa, level);
}

template<>
struct Controller<JumpingKoopaState>{
  static auto run(JumpingKoopaState& koopa, LevelState& level){
    using Type = KoopaState::Type;
    switch(koopa.type){
      case Type::Green: green_jumping_koopa_controller(koopa, level); return;
      case Type::Red: red_jumping_koopa_controller(koopa, level); return;
      case Type::Purple: purple_jumping_koopa_controller(koopa, level); return;
    }
  }
};
