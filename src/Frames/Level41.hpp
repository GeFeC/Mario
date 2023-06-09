#pragma once

#include "LevelGenerator/Builder.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"

namespace mario::frames{

static auto run_level41(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level41;
  level.type = LevelState::Type::Horizontal;
  level.number = { 4, 1 };
  level.extra_textures = {
    texture_groups::red_flying_koopa,
    &textures::dirt,
    &textures::bg,
    &textures::water_top,
    &textures::water_bottom,
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::bg;
    level.biome = LevelState::Biome::Underwater;

    level_generator::generate_level(level, "level41_1.csv");
    level_generator::generate_level(level, "level41_2.csv");
  });
}

} //namespace mario::frames
