#pragma once

#include "LevelGenerator/Builder.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/FireBarState.hpp"
#include "res/textureGroups.hpp"

namespace mario::frames{

static auto run_level33(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level33;
  level.type = LevelState::Type::Vertical;
  level.number = { 3, 3 };
  level.extra_textures = {
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
    level_generator::generate_level(level, "level33_1.csv");
    level_generator::generate_level(level, "level33_2.csv");
    level_generator::generate_level(level, "level33_3.csv");

    level_generator::put_q_block_with_coins(level, { 17, 113 }, 5);
    level_generator::put_q_block_with_entity(level, HammerBroState::make_red({ 9, 31 }));
    level.game_objects.push(PlatformState({ 8, 98 }, { 0, -38 }, 8));
  });
}

} //namespace mario::frames
