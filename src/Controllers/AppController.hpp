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

#include "Frames/Level41.hpp"
#include "Frames/Level42.hpp"
#include "Frames/Level43.hpp"
#include "Frames/Level44.hpp"
#include "Frames/Level45.hpp"
#include "Frames/Level46.hpp"

#include "Frames/Level51.hpp"
#include "Frames/Level52.hpp"
#include "Frames/Level53.hpp"
#include "Frames/Level54.hpp"
#include "Frames/Level55.hpp"
#include "Frames/Level56.hpp"

#include "Frames/Level61.hpp"
#include "Frames/Level62.hpp"
#include "Frames/Level63.hpp"
#include "Frames/Level64.hpp"
#include "Frames/Level65.hpp"

#include "Util/Util.hpp"

#include <unordered_map>

namespace mario::app_controller{

static auto frame_functions = std::unordered_map<AppState::Frame, void(*)(AppState&)>{
  { AppState::Frame::Level11, frames::run_level11 },
  { AppState::Frame::Level12, frames::run_level12 },
  { AppState::Frame::Level13, frames::run_level13 },
  { AppState::Frame::Level14, frames::run_level14 },
  { AppState::Frame::Level15, frames::run_level15 },
  { AppState::Frame::Level16, frames::run_level16 },

  { AppState::Frame::Level21, frames::run_level21 },
  { AppState::Frame::Level22, frames::run_level22 },
  { AppState::Frame::Level23, frames::run_level23 },
  { AppState::Frame::Level24, frames::run_level24 },
  { AppState::Frame::Level25, frames::run_level25 },
  { AppState::Frame::Level26, frames::run_level26 },

  { AppState::Frame::Level31, frames::run_level31 },
  { AppState::Frame::Level32, frames::run_level32 },
  { AppState::Frame::Level33, frames::run_level33 },
  { AppState::Frame::Level34, frames::run_level34 },
  { AppState::Frame::Level35, frames::run_level35 },
  { AppState::Frame::Level36, frames::run_level36 },

  { AppState::Frame::Level41, frames::run_level41 },
  { AppState::Frame::Level42, frames::run_level42 },
  { AppState::Frame::Level43, frames::run_level43 },
  { AppState::Frame::Level44, frames::run_level44 },
  { AppState::Frame::Level45, frames::run_level45 },
  { AppState::Frame::Level46, frames::run_level46 },

  { AppState::Frame::Level51, frames::run_level51 },
  { AppState::Frame::Level52, frames::run_level52 },
  { AppState::Frame::Level53, frames::run_level53 },
  { AppState::Frame::Level54, frames::run_level54 },
  { AppState::Frame::Level55, frames::run_level55 },
  { AppState::Frame::Level56, frames::run_level56 },

  { AppState::Frame::Level61, frames::run_level61 },
  { AppState::Frame::Level62, frames::run_level62 },
  { AppState::Frame::Level63, frames::run_level63 },
  { AppState::Frame::Level64, frames::run_level64 },
  { AppState::Frame::Level65, frames::run_level65 },
};

static auto run(AppState& app){
  auto& frame = app.current_frame;

  const auto& run_level = frame_functions[frame];
  run_level(app);

  //reset level:
  const auto stats = app.current_level.stats;
  const auto player_form = app.current_level.player.form;
  const auto player_growth = app.current_level.player.growth;
  const auto current_checkpoint = app.current_level.current_checkpoint;

  app.current_level = LevelState{};

  //reset stats position:
  auto& new_stats = app.current_level.stats;
  new_stats = stats;
  new_stats.move_direction = util::Direction::up();
  new_stats.position_y = StatsState::MinPositionY;

  app.current_level.stats.boss_hp = StatsState::BossHp();
  app.current_level.player.form = player_form;
  app.current_level.player.growth = player_growth;
  app.current_level.current_checkpoint = current_checkpoint;
}

} //namespace mario::app_controller
