#pragma once

#include "States/AppState.hpp"
#include "Controllers/LevelController.hpp"

#include "Frames/Level11.hpp"
#include "Frames/Level12.hpp"
#include "Frames/Level13.hpp"
#include "Frames/Level14.hpp"
#include "Frames/Level15.hpp"
#include "Frames/Level16.hpp"

#include "Frames/Level21.hpp"
#include "Frames/Level22.hpp"
#include "Frames/Level23.hpp"
#include "Frames/Level24.hpp"
#include "Frames/Level25.hpp"
#include "Frames/Level26.hpp"

#include "Frames/Level31.hpp"
#include "Frames/Level32.hpp"
#include "Frames/Level33.hpp"
#include "Frames/Level34.hpp"
#include "Frames/Level35.hpp"
#include "Frames/Level36.hpp"

#include "Util/Util.hpp"

#include <unordered_map>
#include <functional>

static auto app_update_level(AppState& app){
  const auto stats = app.current_level.stats;
  const auto player_form = app.current_level.player.form;
  const auto player_growth = app.current_level.player.growth;
  const auto current_checkpoint = app.current_level.current_checkpoint;

  app.current_level = LevelState{};

  app.current_level.stats = stats;
  app.current_level.stats.boss_hp = nullptr;
  app.current_level.player.form = player_form;
  app.current_level.player.growth = player_growth;
  app.current_level.current_checkpoint = current_checkpoint;
}

static auto app_frame_functions = std::unordered_map<AppState::Frame, void(*)(AppState&)>{
  { AppState::Frame::Level11, run_frame_level11 },
  { AppState::Frame::Level12, run_frame_level12 },
  { AppState::Frame::Level13, run_frame_level13 },
  { AppState::Frame::Level14, run_frame_level14 },
  { AppState::Frame::Level15, run_frame_level15 },
  { AppState::Frame::Level16, run_frame_level16 },

  { AppState::Frame::Level21, run_frame_level21 },
  { AppState::Frame::Level22, run_frame_level22 },
  { AppState::Frame::Level23, run_frame_level23 },
  { AppState::Frame::Level24, run_frame_level24 },
  { AppState::Frame::Level25, run_frame_level25 },
  { AppState::Frame::Level26, run_frame_level26 },

  { AppState::Frame::Level31, run_frame_level31 },
  { AppState::Frame::Level32, run_frame_level32 },
  { AppState::Frame::Level33, run_frame_level33 },
  { AppState::Frame::Level34, run_frame_level34 },
  { AppState::Frame::Level35, run_frame_level35 },
  { AppState::Frame::Level36, run_frame_level36 },
};

static auto app_controller(AppState& app){
  auto& frame = app.current_frame;

  const auto& run_level = app_frame_functions[frame];
  run_level(app);
  app_update_level(app);
}
