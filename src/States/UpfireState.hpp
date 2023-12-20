#pragma once

#include "States/EntityState.hpp"
#include "States/BlockState.hpp"
#include "Util/LoopedCounter.hpp"

namespace mario{

struct UpfireState : EntityState{
	util::LoopedCounter counter = util::LoopedCounter(10.f, 4.f, 1);
	float initial_y = 0.f;
	float jump_power = 0.f;

	UpfireState(const glm::vec2& position, float jump_power = 25.f){
		this->position = position * BlockBase::Size;
		initial_y = this->position.y;
		this->jump_power = jump_power;
		should_collide = false;
		size = glm::vec2(BlockBase::Size);
		current_texture = &textures::upfire;
		is_active = false;
		can_be_stomped = false;
		gravity_boost = 0.5f;
	}
};

} //namespace mario
