#pragma once

#include "States/LevelState.hpp"

struct AppState{
  enum class Frame{
    Level11, Level12, Level13, Level14, Level15, Level16,
    Level21, Level22, Level23, Level24, Level25, Level26,
    Level31, Level32, Level33, Level34, Level35, Level36
  };

  bool should_restart_current_frame = false;
  Frame current_frame;

  LevelState current_level;
};
