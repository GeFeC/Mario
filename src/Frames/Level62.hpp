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
    level.game_objects.push(LakitoState::make({ 19, 131 }));
    level.game_objects.push(FireBarState({ 17, 145 }, 6));
    level.game_objects.push(LakitoState::make({ 19, 120 }));
    level.game_objects.push(PlatformState({ 16, 114 }, { -16, -10 }, 8));
    level.game_objects.push(PlatformState({ 18, 64 }, { 0, -5 }, 4));

    level.game_objects.push(LakitoState::make({ 19, 105 }));
    level.game_objects.push(LakitoState::make({ 19, 91 }));
    level.game_objects.push(LakitoState::make({ 19, 79 }));
    level.game_objects.push(LakitoState::make({ 0, 62 }));
    level.game_objects.push(LakitoState::make({ 19, 62 }));
    level.game_objects.push(LakitoState::make({ 6, 60 }));
    level.game_objects.push(LakitoState::make({ 13, 60 }));

    level.game_objects.push(FireBarState({ 9, 68 }, 6));
    level.game_objects.push(FireBarState({ 15, 67 }, 6));

    for (int i = 0; i < 5; ++i){
      level.game_objects.push(FlyingKoopaState::make_green({ 13 - i * 2, 57 }, { 0.5f, 0.0f }));
      level.game_objects.push(FlyingKoopaState::make_green({ 13 - i * 2, 53 }, { 0.5f, 0.0f }));
    }

    for (int i = 0; i < 9; ++i){
      level.game_objects.push(LakitoState::make({ 2 + i, 10 + i }));
    }

    create_hitbox_on_sides(level);
    level_generator::generate_vertical_level_clouds(level);
    level_generator::generate_level(level, "level62_1.csv");
    level_generator::generate_level(level, "level62_2.csv");
  });
}

} //namespace mario::frames
