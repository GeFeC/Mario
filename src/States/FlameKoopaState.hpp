#pragma once

#include "States/MonsterState.hpp"
#include "States/FireballState.hpp"
#include "States/KoopaState.hpp"

namespace mario{

struct FlameKoopaBase{
	FireballState fireball;
};

struct FlameKoopaState : ShellMonsterState, FlameKoopaBase{
	static auto make(const glm::vec2& position){
		auto koopa = KoopaState::make<FlameKoopaState>(position);
		koopa.current_texture = &textures::flame_koopa_walk[0];
		koopa.fall_from_edge = false;
		koopa.follows_player = true;
		koopa.reward_for_killing = 400;

		return koopa;
	}
};

} //namespace mario
