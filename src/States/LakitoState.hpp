#pragma once

#include "States/MonsterState.hpp"
#include "Util/LoopedCounter.hpp"
#include "res/textures.hpp"

namespace mario{

struct LakitoState : MonsterState{
  struct ThrownSpikeState : MonsterState{
    static auto make(){
      auto spike = ThrownSpikeState{};
      spike.size = glm::vec2(BlockBase::Size);
      spike.current_texture = &textures::lakito_throw[0];
      spike.position.y = util::BigValue;

      return spike;
    }
  } thrown_spike;

  util::InfiniteCounter throw_counter = util::InfiniteCounter(10.f, 3.f);
  bool should_throw_spike = false;

  auto throw_spike(ThrownSpikeState& spike, float power){
    spike.position = position + glm::vec2(0.f, gravity_flip.is_flipped() * size.y - spike.size.y);

    spike.is_active = true;
    spike.gravity = -power;
    spike.gravity_flip = gravity_flip;
    spike.is_on_ground = false;
  }

  auto throw_spike(float power){
    throw_spike(thrown_spike, power);
  }

  static auto make(const glm::vec2& position){
    auto lakito = LakitoState{};
    lakito.current_texture = &textures::lakito_hidden;
    lakito.size = glm::vec2(1.f, 1.5f) * BlockBase::Size;
    lakito.position = position * BlockBase::Size;
    lakito.reward_for_killing = 2000;
    lakito.should_collide = false;

    lakito.thrown_spike = ThrownSpikeState::make();

    return lakito;
  }
};

} //namespace mario
