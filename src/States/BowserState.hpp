#pragma once

#include "States/MonsterState.hpp"
#include "States/BlockState.hpp"

#include "Util/LoopedCounter.hpp"

namespace mario{

struct BowserState : MonsterState{
	static constexpr auto WalkSpeed = 2.f;

	struct FireState : EntityState{
		glm::vec2 direction_vec;

		FireState(){
			size = glm::vec2(1.5f, 0.5f) * BlockBase::Size;
			current_texture = &textures::bowser_fire[0];
		}
	};

	util::Generator<FireState> fire_generator;
	util::LoopedCounter jump_counter = util::LoopedCounter(4.f, 1.f, 1);
	util::LoopedCounter attack_counter = util::LoopedCounter(2.f, 1.5f, 1);
	bool is_attacking = false;
	bool is_highlighted = false;
	int hp = 10;
	float initial_x = 0.f;

	static auto make(const glm::vec2& position){
		auto bowser = BowserState{};
		bowser.reward_for_killing = 10'000;
		bowser.acceleration.right = WalkSpeed;
		bowser.position = position * BlockBase::Size;
		bowser.initial_x = bowser.position.x;
		bowser.size = glm::vec2(BlockBase::Size * 2.f);
		bowser.current_texture = &textures::bowser_walk[0];
		bowser.can_be_stomped = false;
		bowser.gravity_boost = 0.5f;
		bowser.texture_flip = util::Flip::flip();

		return bowser;
	}
};

} //namespace mario
