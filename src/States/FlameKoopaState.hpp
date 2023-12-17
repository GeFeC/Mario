#pragma once

#include "States/MonsterState.hpp"
#include "States/FireballState.hpp"
#include "States/KoopaState.hpp"

namespace mario{

struct FlameKoopaState : ShellMonsterState{
	FireballState fireball;

	static auto make(const glm::vec2& position){
		auto koopa = KoopaState::make<FlameKoopaState>(position);
		koopa.current_texture = &textures::flame_koopa_walk[0];
		koopa.fall_from_edge = false;
		koopa.follows_player = true;

		return koopa;
	}
};

} //namespace mario
