#pragma once

#include "States/FlameKoopaState.hpp"
#include "States/LevelState.hpp"

#include "Controllers/MonsterController.hpp"
#include "Controllers/ShellMonsterController.hpp"

namespace mario{

static auto run_controller(FlameKoopaState& koopa, LevelState& level){
	monster_controller::endure_fireball(koopa, level.player);
	monster_controller::become_active_when_seen(koopa, level);
  monster_controller::die_when_on_bouncing_block(koopa, level);

	shell_monster_controller::run(koopa, level, textures::flame_koopa_walk);
	shell_monster_controller::handle_shell(koopa, level, textures::flame_koopa_dead);

	if (!koopa.in_shell){
		monster_controller::follow_player(koopa, level);
	}

	monster_controller::throw_fireball(koopa, level);
}

} //namespace mario
