#pragma once

#include "States/AppState.hpp"
#include "Controllers/LevelController.hpp"

#include "Util.hpp"
#include "config.hpp"

auto app_controller(AppState& app_state){
  const auto& player = app_state.current_level.player;

  if (player.position.y > config::PlayerPositionToRestartLevel){
    app_state.should_restart_current_frame = true;
  }
}
