#include "States/LevelState.hpp"
#include "Controllers/JumpingKoopaController.hpp"

namespace mario{

static auto run_controller(FlameJumpingKoopaState& koopa, LevelState& level){
	jumping_koopa_controller::run_controller_base(
		koopa,
		level,
		textures::flame_flying_koopa_walk,
		textures::flame_koopa_walk,
		true
	);

	shell_monster_controller::handle_shell(koopa, level, textures::flame_koopa_dead);

	if (!koopa.in_shell && koopa.position.y >= level.player.position.y - koopa.size.y){
		monster_controller::follow_player(koopa, level);
	}

	monster_controller::throw_fireball(koopa, level);
}

} //namespace mario
