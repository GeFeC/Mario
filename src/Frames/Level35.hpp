#pragma once

#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/FireBarState.hpp"
#include "res/textureGroups.hpp"

namespace mario::frames{

static auto run_level35(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level35;
  level.type = LevelState::Type::Vertical;
  level.number = { 3, 5 };
  level.extra_textures = {
    texture_groups::green_flying_koopa,
    texture_groups::blue_cloud,
    &textures::platform,
    &textures::snow_bg
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::snow_bg;
    level.cloud_textures = &texture_groups::blue_cloud;
    level.player.slip = 1.5f;

    level_generator::generate_vertical_level_clouds(level);
    level_generator::generate_level(level, "level35_1.csv");
    level_generator::generate_level(level, "level35_2.csv");

    auto& objects = level.game_objects;
    objects.push(FireBarState({ 16, 133 }, 6));
    
    for (int i = 0; i < 10; ++i){
      objects.push(FlyingKoopaState::make_green({ 9.5f, 102 + i * 4 }, { 0.5f, 0.f }));
    }
  });
}

} //namespace mario::frames
