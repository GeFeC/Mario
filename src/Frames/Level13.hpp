#pragma once

#include "States/AppState.hpp"
#include "States/HammerBroState.hpp"
#include "Views/Level.hpp"
#include "Controllers/LevelController.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

static auto run_frame_level13(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level13;
  level.type = LevelState::Type::Vertical;
  level.number = { 1, 3 };
  level.extra_textures = {
    texture_groups::red_cloud,
    &textures::mushroom_bg
  };

  run_frame_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::mushroom_bg;
    level.cloud_textures = &texture_groups::red_cloud;

    level.fire_bars.push_back(FireBarState({ 13, 133 }, 3));
    level.fire_bars.push_back(FireBarState({ 5, 104 }, 5));
    level.fire_bars.push_back(FireBarState({ 6, 61 }, 3));
    level.fire_bars.push_back(FireBarState({ 11, 58 }, 3));
    level.fire_bars.push_back(FireBarState({ 16, 55 }, 3));
    level.fire_bars.push_back(FireBarState({ 7, 45 }, 7));
    level.fire_bars.push_back(FireBarState({ 7, 24 }, 5));
    level.fire_bars.push_back(FireBarState({ 4, 4 }, 7));

    level_generator::put_q_block_with_coins(level, { 4, 65 }, 5);

    level_generator::generate_vertical_level_clouds(level);
    level_generator::generate_level(level, "level13_1.csv");
    level_generator::generate_level(level, "level13_2.csv");
  });
}
