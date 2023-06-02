#pragma once

#include "Window.hpp"
#include "Util/Util.hpp"

namespace util{

struct LoopedCounter{
  float value = 0;

  float limit = 1;
  float step = 1;

  int max_loops = 1;
  int loops = 0;

  LoopedCounter(float limit, float step, int max_loops)
  : limit(limit), step(step), max_loops(max_loops) {}

  auto int_value() const{
    return value | util::as<int>; 
  }

  auto stopped_counting() const{
    return loops == max_loops;
  }

  auto run(){
    if (loops == max_loops) return;

    value += step * window::delta_time;
    if (value >= limit){
      value = 0.f;
      ++loops;
    }
  }

  auto reset(){
    loops = 0;
    value = 0.f;
  }

  auto disable(){
    loops = max_loops;
  }
};

struct InfiniteCounter : LoopedCounter{
  InfiniteCounter(float limit, float step) : LoopedCounter(limit, step, 0) {}

  auto run(){
    value += step * window::delta_time;
    if (value >= limit){
      value = 0.f;
      ++loops;
    }
  }
};

} //namespace util
