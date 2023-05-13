#pragma once

#include "LevelGenerator/Builder.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"

static auto run_frame_level21(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level21;
  level.type = LevelState::Type::Horizontal;
  level.number = { 2, 1 };
  level.extra_textures = {
    texture_groups::blue_cloud,
    texture_groups::red_flying_koopa,
    &textures::dirt,
    &textures::bg
  };

  run_frame_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::bg;
    level.cloud_textures = &texture_groups::blue_cloud;

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level21_1.csv");
    level_generator::generate_level(level, "level21_2.csv");

    level.entities.red_flying_koopas.push_back(FlyingKoopaState::make_red({ 79, 6 }, { 0, 2 }));
    level.entities.red_flying_koopas.push_back(FlyingKoopaState::make_red({ 84, 6 }, { 0, 2 }));
    level_generator::put_q_block_with_coins(level, { 150, 8 }, 10);
  });
}
