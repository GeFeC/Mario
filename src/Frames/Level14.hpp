#pragma once

#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"

static auto run_frame_level14(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level14;
  level.type = LevelState::Type::Horizontal;
  level.number = { 1, 4 };
  level.extra_textures = {
    texture_groups::red_cloud,
    &textures::mushroom_bg
  };

  run_frame_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::mushroom_bg;

    level.player.position.x = 40 * config::BlockSize;

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level14_1.csv");
    level_generator::generate_level(level, "level14_2.csv");
    level_generator::generate_level(level, "level14_3.csv");

    level.fire_bars.push_back(FireBarState({ 10, 8 }, 3));
    level.fire_bars.push_back(FireBarState({ 17, 6 }, 3));
    level.fire_bars.push_back(FireBarState({ 24, 4 }, 3));
    level.fire_bars.push_back(FireBarState({ 121, 7 }, 5));
    level.fire_bars.push_back(FireBarState({ 103, 6 }, 5));
    level.fire_bars.push_back(FireBarState({ 108, 6 }, 5));
    level.fire_bars.push_back(FireBarState({ 184, 7 }, 5));

    level_generator::put_qblock_with_coins(level, { 184, 3 }, 5);
  });

}
