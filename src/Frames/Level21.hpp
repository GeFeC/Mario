#pragma once

#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"

static auto run_frame_level21(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level21;
  level.type = LevelState::Type::Horizontal;
  level.number = { 2, 1 };
  level.extra_textures = {
    texture_groups::red_cloud,
    &textures::dirt,
    &textures::bg
  };

  run_frame_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::bg;

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level21_1.csv");
    level_generator::generate_level(level, "level21_2.csv");
  });
}
