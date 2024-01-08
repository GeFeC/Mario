#pragma once

#include "States/EntityState.hpp"
#include "States/BlockState.hpp"
#include "Util/Generator.hpp"
#include "Util/LoopedCounter.hpp"
#include "Util/Util.hpp"

namespace mario{

struct FlameParticleState : EntityState{
  float opacity = 1.f;
  float angle = 0.f;

  FlameParticleState(){
    is_active = true;
    is_visible = true;
  }
};

struct FlameState{
  bool is_active = false;
  glm::vec2 position;
	float rotation = 0.f;
	float speed = 500.f;

	float min_angle = -90.f;
	float max_angle = 90.f;
	
	float opacity_required_to_kill_player = 0.5f;

	float particle_size = BlockBase::Size / 2.f;
	float burn_speed = 2.f;

  util::Generator<FlameParticleState> particle_generator;
  util::LoopedCounter particle_counter = util::LoopedCounter(1.f, 20.f, util::BigValue);

  FlameState(){
    particle_generator.items.reserve(0);
  }

  auto& particles(){
    return particle_generator.items;
  }

  const auto& particles() const{
    return const_cast<FlameState*>(this)->particles();
  }
};

} //namespace mario
