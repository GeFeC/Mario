#pragma once

#include "Util/Direction.hpp"
#include "States/BlockState.hpp"

namespace mario{

struct StatsState{
  struct BossHp{
    int* current = nullptr;
    int max = 0;
  } boss_hp;

  static constexpr auto MinPositionY = 2.f / 3.f * BlockBase::Size;
  static constexpr auto MaxPositionY = config::FrameBufferSize.y - MinPositionY * 4;

  int hp = 0;
  int score = 0;
	int stored_mushrooms = 0;
  int time = 400;
  int coins = 0;
  int level_major = 1;
  int level_minor = 1;

  float position_y = 2.f / 3.f * BlockBase::Size;
  util::Direction move_direction = util::Direction::up();
};

} //namespace mario
