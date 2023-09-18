#pragma once

#include "States/MonsterState.hpp"
#include "Util/LoopedCounter.hpp"
#include "res/textures.hpp"

namespace mario{

struct LakitoState : MonsterState{
  struct ThrownSpikeState : MonsterState{} thrown_spike;

  util::InfiniteCounter throw_counter = util::InfiniteCounter(10.f, 3.f);
  bool was_spike_thrown = false;

  static auto make(const glm::vec2& position){
    auto lakito = LakitoState{};
    lakito.current_texture = &textures::lakito_hidden;
    lakito.size = glm::vec2(1.f, 1.5f) * BlockBase::Size;
    lakito.position = position * BlockBase::Size;
    lakito.reward_for_killing = 2000;
    lakito.should_collide = false;

    lakito.thrown_spike.size = glm::vec2(BlockBase::Size);
    lakito.thrown_spike.current_texture = &textures::lakito_throw[0];
    lakito.thrown_spike.position.y = util::BigValue;

    return lakito;
  }
};

} //namespace mario
