#pragma once

#include "States/LevelState.hpp"
#include "Controllers/FlyingKoopaController.hpp"

namespace mario{

static auto run_controller(FlameFlyingKoopaState& koopa, LevelState& level){
	flying_koopa_controller::run_controller_base(
		koopa,
		level,
		textures::flame_flying_koopa_walk,
		textures::flame_koopa_walk,
		LevelState::timer,
		true
	);

	shell_monster_controller::handle_shell(koopa, level, textures::flame_koopa_dead);

	if (!koopa.in_shell){
		monster_controller::follow_player(koopa, level);
	}

	monster_controller::throw_fireball(koopa, level);
}

} //namespace mario
