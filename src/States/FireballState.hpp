#pragma once

#include "States/LoopedCounter.hpp"
#include "States/BlockState.hpp"
#include "EntityState.hpp"

struct FireballState : EntityState{
  struct ExplosionState : BlockBase{
    LoopedCounter counter;

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

      texture = &textures::explosion[counter.as_int()];
    }

    auto finished() const{
      return !started || counter.stopped_counting();
    }

  } explosion;

  FireballState(){
    position = glm::vec2(0);
    size = glm::vec2(config::BlockSize / 2);
    current_texture = &textures::fireball[0];
    is_visible = false;
  }
};
