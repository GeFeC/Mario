#pragma once

#include "States/AppState.hpp"

#include "Frames/Menu.hpp"

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
#include "Frames/Level66.hpp"

#include "Frames/Level71.hpp"
#include "Frames/Level72.hpp"
#include "Frames/Level73.hpp"
#include "Frames/Level74.hpp"
#include "Frames/Level75.hpp"
#include "Frames/Level76.hpp"

#include <unordered_map>

namespace mario::app_controller{

static auto frame_functions = std::unordered_map<AppState::Frame, void(*)(AppState&)>{
  { AppState::Frame::Menu, frames::run_menu },

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
  { AppState::Frame::Level66, frames::run_level66 },

  { AppState::Frame::Level71, frames::run_level71 },
  { AppState::Frame::Level72, frames::run_level72 },
  { AppState::Frame::Level73, frames::run_level73 },
  { AppState::Frame::Level74, frames::run_level74 },
  { AppState::Frame::Level75, frames::run_level75 },
  { AppState::Frame::Level76, frames::run_level76 },
};

static auto save_progress(AppState& app){
	auto& frame = app.current_frame;
	if (frame == AppState::Frame::Menu) return;

	const auto frame_index = frame | util::as<int>;
	auto state = saves::SaveState{};
	state.world = frame_index / 6;
	state.difficulty = app.difficulty;

	std::transform(input::controls.begin(), input::controls.end(), state.controls.begin(), [](const input::Key& key){
		return key.code;
	});

	if (app.difficulty == AppState::Difficulty::Easy){
		state.level = frame_index % 6;
	}

	saves::save(state);
}

static auto reset_level(AppState& app){
	auto& level = app.current_level;

  const auto prev_stats = level.stats;
  const auto prev_player_form = level.player.form;
  const auto prev_player_growth = level.player.growth;
  const auto prev_current_checkpoint = level.current_checkpoint;

  level = LevelState{};

  auto& stats = level.stats;
  stats = prev_stats;
  stats.move_direction = util::Direction::up();
  stats.position_y = StatsState::MinPositionY;

  level.stats.boss_hp = StatsState::BossHp();
  level.player.form = prev_player_form;
  level.player.growth = prev_player_growth;
  level.current_checkpoint = prev_current_checkpoint;

	if (level.stats.hp == 0){
		const auto prev_level_major = level.stats.level_major;
		const auto prev_level_minor = level.stats.level_minor;

		level.stats = StatsState{};
		level.current_checkpoint = LevelState::CheckpointNotSet;
		level.stats.level_major = prev_level_major;

		if (app.difficulty == AppState::Difficulty::Easy){
			level.stats.level_minor = prev_level_minor;
		}
		else{
			app.current_frame = level_controller::get_worlds_first_level(app.current_frame);
		}
	}
}

static auto run(AppState& app){
  auto& frame = app.current_frame;

	save_progress(app);

  const auto& run_frame = frame_functions[frame];
  run_frame(app);

	if (frame != AppState::Frame::Menu){
		reset_level(app);
	}
}

} //namespace mario::app_controller
