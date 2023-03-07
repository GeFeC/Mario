#pragma once

#include "States/AppState.hpp"
#include "Controllers/LevelController.hpp"
#include "Frames/Level11.hpp"
#include "Frames/Level12.hpp"

#include "Util/Util.hpp"
#include "config.hpp"

static auto app_update_level(AppState& app){
  const auto stats = app.current_level.stats;
  const auto player_form = app.current_level.player.form;
  const auto player_growth = app.current_level.player.growth;

  app.current_level = LevelState{};

  app.current_level.stats = stats;
  app.current_level.player.form = player_form;
  app.current_level.player.growth = player_growth;
}

static auto app_controller(AppState& app){
  auto& frame = app.current_frame;

  if (frame == AppState::Frame::Level11){
    run_frame_level11(app);
    app_update_level(app);
  }

  if (frame == AppState::Frame::Level12){
    run_frame_level12(app);
    app_update_level(app);
  }
}
