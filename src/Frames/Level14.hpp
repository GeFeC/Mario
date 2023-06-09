#pragma once

#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"

namespace mario::frames{

static auto run_level14(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level14;
  level.type = LevelState::Type::Horizontal;
  level.number = { 1, 4 };
  level.extra_textures = {
    texture_groups::red_cloud,
    &textures::mushroom_bg
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::mushroom_bg;
    level.cloud_textures = &texture_groups::red_cloud;

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level14_1.csv");
    level_generator::generate_level(level, "level14_2.csv");
    level_generator::generate_level(level, "level14_3.csv");

    auto& objects = level.game_objects;
    objects.push(FireBarState({ 10, 8 }, 3));
    objects.push(FireBarState({ 17, 6 }, 3));
    objects.push(FireBarState({ 24, 4 }, 3));
    objects.push(FireBarState({ 121, 7 }, 5));
    objects.push(FireBarState({ 103, 6 }, 5));
    objects.push(FireBarState({ 108, 6 }, 5));
    objects.push(FireBarState({ 184, 7 }, 5));

    level_generator::put_q_block_with_coins(level, { 184, 3 }, 5);
  });

}

} //namespace mario::frames
