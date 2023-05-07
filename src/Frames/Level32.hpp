#pragma once

#include "LevelGenerator/Builder.hpp"

#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"

static auto run_frame_level32(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level32;
  level.type = LevelState::Type::Horizontal;
  level.number = { 3, 2 };
  level.extra_textures = {
    texture_groups::blue_cloud,
    texture_groups::purple_flying_koopa,
    &textures::platform,
    &textures::dirt,
    &textures::snow_bg
  };

  run_frame_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::snow_bg;
    level.cloud_textures = &texture_groups::blue_cloud;
    level.player.slip = 1.5f;

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level32_1.csv");
    level_generator::generate_level(level, "level32_2.csv");

    level_generator::put_q_block_with_entity(level, { 11, 7 }, [&](const auto& position){
      return GoombaState::make_normal(position);
    });
  });
}
