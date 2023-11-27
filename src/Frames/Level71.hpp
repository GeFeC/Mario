#pragma once

#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

namespace mario::frames{

static auto run_level71(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level71;
  level.type = LevelState::Type::Horizontal;
  level.number = { 7, 1 };
  level.extra_textures = {
    &textures::castle_bg,
    texture_groups::flame_goomba
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::castle_bg;

    level_generator::generate_level(level, "level71_1.csv");
    level_generator::generate_level(level, "level71_2.csv");
  });
}

} //namespace mario::frames
