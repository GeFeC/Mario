#pragma once

#include "States/LevelState.hpp"

struct AppState{
  enum class Frame{
    Level11, Level12, Level13, Level14, Level15
  };

  bool should_restart_current_frame = false;
  Frame current_frame;

  LevelState current_level;
};
