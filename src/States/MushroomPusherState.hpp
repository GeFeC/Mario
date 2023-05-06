#pragma once

#include "States/MushroomState.hpp"

struct MushroomPusherState{
  MushroomState mushroom;

  enum class MushroomType{
    Red, Green
  } mushroom_type = MushroomType::Red;
};
