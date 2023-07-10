#pragma once

#include "States/AppState.hpp"
#include "Views/Level.hpp"
#include "Controllers/LevelController.hpp"
#include "Frame.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"
#include "Frames/LevelBase.hpp"

namespace mario::frames{

static auto run_level51(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level51;
  level.type = LevelState::Type::Horizontal;
  level.number = { 5, 1 };
  level.extra_textures = {
    &textures::underground_bg
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::underground_bg;

    level.game_objects.push(FireBarState({ 65, 6 }, 4));
    level.game_objects.push(FireBarState({ 194, 6 }, 8));
    
    level_generator::generate_level(level, "level51_1.csv");
    level_generator::generate_level(level, "level51_2.csv");
  });
}

} //namespace mario::frames
