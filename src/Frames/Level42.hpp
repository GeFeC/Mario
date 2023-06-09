#pragma once

#include "LevelGenerator/Builder.hpp"
#include "LevelGenerator/LevelGenerator.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"

namespace mario::frames{

static auto run_level42(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level42;
  level.type = LevelState::Type::Horizontal;
  level.number = { 4, 2 };
  level.extra_textures = {
    texture_groups::red_flying_koopa,
    texture_groups::blue_cloud,
    texture_groups::red_fish,
    &textures::dirt,
    &textures::bg,
    &textures::water_top,
    &textures::water_bottom,
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::bg;
    level.cloud_textures = &texture_groups::blue_cloud;
    level.biome = LevelState::Biome::Land;
    level.water_level = LevelState::BlocksInColumn - 1;

    for (int i = 22; i < LevelState::HorizontalLevelSize.x; i += FishState::random_value(5, 8)){
      level.game_objects.push(FishState::make_jumping({ i, 12.f }));
    }

    level_generator::put_q_block_with_coins(level, { 140, 2 }, 10);
    level_generator::put_q_block_with_coins(level, { 160, 2 }, 10);

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level42_1.csv");
    level_generator::generate_level(level, "level42_2.csv");
  });
}

} //namespace mario::frames
