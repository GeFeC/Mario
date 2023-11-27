#pragma once

#include "States/FlameState.hpp"
#include "States/BlockState.hpp"
#include "States/FireballState.hpp"
#include "States/MonsterState.hpp"
#include "Util/Util.hpp"
#include "res/textures.hpp"

#include "Util/Generator.hpp"
#include "Util/LoopedCounter.hpp"

namespace mario{

struct FlameGoombaState : MonsterState{
  FlameState inner_flame;
  util::Generator<FlameState> flames_generator;
  util::LoopedCounter flame_put_counter = util::LoopedCounter(1.f, 5.f, util::BigValue);

  static auto make(const glm::vec2& position){
    auto goomba = FlameGoombaState();
    goomba.position = position * BlockBase::Size;
    goomba.size = glm::vec2(BlockBase::Size);
    goomba.walk_speed = 4.f;
    goomba.current_texture = &textures::flame_goomba_walk[0];
    goomba.can_be_stomped = false;

    return goomba;
  }
};

} //namespace mario
