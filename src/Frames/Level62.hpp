#pragma once

#include "LevelGenerator/Builder.hpp"
#include "LevelGenerator/LevelGenerator.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/FlyingKoopaState.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

namespace mario::frames{

static auto run_level62(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level62;
  level.type = LevelState::Type::Vertical;
  level.number = { 6, 2 };
  level.extra_textures = {
    &textures::sky_bg,
    &textures::platform,
    texture_groups::blue_cloud,
    texture_groups::purple_flying_koopa,
    texture_groups::green_flying_koopa,
    texture_groups::lakito,
    texture_groups::spike
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::sky_bg;
    level.cloud_textures = &texture_groups::blue_cloud;

    level.game_objects.push(FlyingKoopaState::make_purple({ 7, 144 }, { 0, 4 }));
    level.game_objects.push(FlyingKoopaState::make_purple({ 12, 144 }, { 0, -4 }));
    level.game_objects.push(FireBarState({ 17, 145 }, 6));
    level.game_objects.push(PlatformState({ 9, 114 }, { -7, -7 }, 6));
    level.game_objects.push(PlatformState({ 18, 90 }, { 0, -5 }, 4));

    level.game_objects.push(LakitoState::make({ 19, 131 }));
    level.game_objects.push(LakitoState::make({ 19, 120 }));
    level.game_objects.push(LakitoState::make({ 19, 109 }));
    level.game_objects.push(LakitoState::make({ 0, 88 }));
    level.game_objects.push(LakitoState::make({ 19, 88 }));
    level.game_objects.push(LakitoState::make({ 6, 86 }));
    level.game_objects.push(LakitoState::make({ 13, 86 }));

    level.game_objects.push(FireBarState({ 9, 94 }, 6));
    level.game_objects.push(FireBarState({ 15, 93 }, 6));

    for (int i = 0; i < 5; ++i){
      level.game_objects.push(FlyingKoopaState::make_green({ 13 - i * 2, 79 }, { 0.5f, 0.0f }));
      level.game_objects.push(FlyingKoopaState::make_green({ 13 - i * 2, 83 }, { 0.5f, 0.0f }));
    }

    for (int i = 0; i < 9; ++i){
      level.game_objects.push(LakitoState::make({ 2 + i, 59 + i }));
    }

    level_generator::generate_vertical_level_clouds(level);
    level_generator::generate_level(level, "level62_1.csv");
    level_generator::generate_level(level, "level62_2.csv");
  });
}

} //namespace mario::frames
