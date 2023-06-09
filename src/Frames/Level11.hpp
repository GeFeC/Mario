#pragma once

#include "States/AppState.hpp"
#include "States/HammerBroState.hpp"
#include "Views/Level.hpp"
#include "Controllers/LevelController.hpp"
#include "Frame.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"
#include "Frames/LevelBase.hpp"

namespace mario::frames{

static auto run_level11(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level11;
  level.type = LevelState::Type::Horizontal;
  level.number = { 1, 1 };
  level.extra_textures = {
    texture_groups::red_cloud,
    &textures::mushroom_bg
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::mushroom_bg;
    level.cloud_textures = &texture_groups::red_cloud;
    
    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level11_1.csv");
    level_generator::generate_level(level, "level11_2.csv");
    level_generator::generate_level(level, "level11_3.csv");

    level_generator::put_q_block_with_coins(level, { 77, 7 }, 5);
    level_generator::put_q_block_with_coins(level, { 187, 3 }, 5);
    level_generator::put_q_block_with_coins(level, { 189, 3 }, 5);
  });
}

} //namespace mario::frames
