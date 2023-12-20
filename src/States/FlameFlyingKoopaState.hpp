#pragma once

#include "States/FlyingKoopaState.hpp"
#include "States/FlameKoopaState.hpp"

namespace mario{

struct FlameFlyingKoopaState : FlyingKoopaState, FlameKoopaBase{
	static auto make(const glm::vec2& position, const glm::vec2& axis){
    auto koopa = FlyingKoopaState::make<FlameFlyingKoopaState>(position, axis);
		koopa.current_texture = &textures::flame_flying_koopa_walk[0];

    return koopa;
	}

	auto flip_gravity(){
		EntityState::flip_gravity();
		fireball.flip_gravity();
	}
};

} //namespace mario
