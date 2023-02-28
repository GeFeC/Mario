#pragma once

#include "States/AppState.hpp"
#include "Controllers/LevelController.hpp"
#include "Frames/Level11.hpp"

#include "Util/Util.hpp"
#include "config.hpp"

static auto app_controller(AppState& app){
  auto& frame = app.current_frame;

  if (frame == AppState::Frame::Level11){
    run_frame_level11(app);

    const auto player_hp = app.current_level.stats.hp - 1;

    app.current_level = LevelState{};
    app.current_level.stats.hp = player_hp;
  }
}
