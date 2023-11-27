#pragma once

#include "States/EntityState.hpp"
#include "States/BlockState.hpp"
#include "Util/Generator.hpp"
#include "Util/LoopedCounter.hpp"
#include "Util/Random.hpp"
#include "Util/Util.hpp"
#include "Window.hpp"
#include <iostream>

namespace mario{

struct FlameParticleState : EntityState{
  static constexpr auto Size = glm::vec2(BlockBase::Size / 2.f);

  float opacity = 1.f;
  float angle = 0.f;

  FlameParticleState(){
    size = Size;
    is_active = true;
    is_visible = true;
  }
};

struct FlameState{
  bool is_active = false;
  glm::vec2 position;
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
