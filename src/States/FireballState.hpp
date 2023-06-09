#pragma once

#include "Util/LoopedCounter.hpp"
#include "States/BlockState.hpp"
#include "EntityState.hpp"

namespace mario{

struct ExplosionState : BlockBase{
  util::LoopedCounter counter;

  bool started = false;

  ExplosionState() : counter(3.f, 10.f, 1) {
    texture = &textures::explosion[0];
    is_visible = false;
  }

  auto start(const glm::vec2& position){
    started = true;
    is_visible = true;
    this->position = position;
  }

  auto run(){
    if (!started) return;
    counter.run();

    if (counter.stopped_counting()) {
      is_visible = false;
      started = false;
      counter.reset();
      return;
    }

    texture = &textures::explosion[counter.int_value()];
  }

  auto finished() const{
    return !started || counter.stopped_counting();
  }
};


struct FireballState : EntityState{
  inline static constexpr auto BouncePower = -12.f;

  ExplosionState explosion;

  FireballState(){
    position = glm::vec2(-util::BigValue);
    size = glm::vec2(BlockBase::Size / 2);
    current_texture = &textures::fireball[0];
    is_visible = false;
  }
};

} //namespace mario
