#pragma once

#include "States/BlockState.hpp"
#include "States/MonsterState.hpp"
#include "Util/LoopedCounter.hpp"
#include "Util/Generator.hpp"

namespace mario{

struct CannonState : BlockState{
	struct BulletState : MonsterState{
		explicit BulletState(const glm::vec2& position){
			current_texture = &textures::cannon_bullet;
			this->position = position * BlockBase::Size;
			size = glm::vec2(BlockBase::Size);
			walk_speed = 8.f;
			reward_for_killing = 100;
		}
	};

	util::Generator<BulletState> bullet_generator;
	util::LoopedCounter shot_counter = util::LoopedCounter(6.f, 1.f, 1);

	explicit CannonState(const glm::vec2& position)
		: BlockState(position, &textures::cannon_top)
	{}
};

} //namespace mario
