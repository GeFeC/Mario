#pragma once

#include "LevelGenerator/Builder.hpp"
#include "States/AppState.hpp"
#include "States/FireBarState.hpp"
#include "States/FlyingKoopaState.hpp"
#include "States/LakitoState.hpp"
#include "Views/Level.hpp"
#include "Controllers/LevelController.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"

namespace mario::frames{

static auto run_level64(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level64;
  level.type = LevelState::Type::Horizontal;
  level.number = { 6, 4 };
  level.extra_textures = {
    texture_groups::blue_cloud,
    texture_groups::lakito,
    texture_groups::spike,
    &textures::sky_bg
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::sky_bg;
    level.cloud_textures = &texture_groups::blue_cloud;

    level_generator::generate_horizontal_level_clouds(level, 10);
    level_generator::generate_level(level, "level64_1.csv");
    level_generator::generate_level(level, "level64_2.csv");
  });
}

} //namespace mario::frames
