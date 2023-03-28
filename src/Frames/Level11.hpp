#pragma once

#include "States/AppState.hpp"
#include "States/HammerBroState.hpp"
#include "Views/Level.hpp"
#include "Controllers/LevelController.hpp"
#include "Frame.hpp"
#include "LevelGenerator.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"
#include "Frames/LevelBase.hpp"

static auto run_frame_level11(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level11;
  level.type = LevelState::Type::Horizontal;
  level.number = { 1, 1 };
  level.extra_textures = {
    texture_groups::red_cloud,
    &textures::mushroom_bg
  };

  run_frame_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level11_1.csv");
    level_generator::generate_level(level, "level11_2.csv");
    level_generator::generate_level(level, "level11_3.csv");
    level_generator::put_qblock_with_coins(level, { 77, 7 }, 5);
    level_generator::put_qblock_with_coins(level, { 187, 3 }, 5);
    level_generator::put_qblock_with_coins(level, { 189, 3 }, 5);
  });
}
