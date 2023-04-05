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

    level.fire_bars.push_back(FireBarState({ 13, 183 }, 3));
    level.fire_bars.push_back(FireBarState({ 5, 154 }, 5));
    level.fire_bars.push_back(FireBarState({ 6, 111 }, 3));
    level.fire_bars.push_back(FireBarState({ 11, 108 }, 3));
    level.fire_bars.push_back(FireBarState({ 16, 105 }, 3));
    level.fire_bars.push_back(FireBarState({ 7, 95 }, 7));
    level.fire_bars.push_back(FireBarState({ 7, 74 }, 5));
    level.fire_bars.push_back(FireBarState({ 16, 39 }, 5));
    level.fire_bars.push_back(FireBarState({ 4, 54 }, 7));

    level_generator::put_qblock_with_coins(level, { 7, 25 }, 5);
    level_generator::put_qblock_with_coins(level, { 4, 115 }, 5);

    level_generator::generate_vertical_level_clouds(level);
    level_generator::generate_level(level, "level13_1.csv");
    level_generator::generate_level(level, "level13_2.csv");
  });
}
