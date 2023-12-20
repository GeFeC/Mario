#pragma once

#include "States/JumpingKoopaState.hpp"
#include "States/FlameKoopaState.hpp"

namespace mario{

struct FlameJumpingKoopaState : JumpingKoopaState, FlameKoopaBase{
	static auto make(const glm::vec2& position){
    auto koopa = JumpingKoopaState::make<FlameJumpingKoopaState>(position);
		koopa.current_texture = &textures::flame_flying_koopa_walk[0];

    return koopa;
	}

	auto flip_gravity(){
		EntityState::flip_gravity();
		fireball.flip_gravity();
	}
};

} //namespace mario
