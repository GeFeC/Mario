#pragma once
#include "LevelGenerator/Builder.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/FireBarState.hpp"
#include "res/textureGroups.hpp"

static auto run_frame_level24(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level24;
  level.type = LevelState::Type::Vertical;
  level.number = { 2, 4 };
  level.extra_textures = {
    texture_groups::blue_cloud,
    texture_groups::red_flying_koopa,
    texture_groups::purple_flying_koopa,
    &textures::platform,
    &textures::bg
  };

  run_frame_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::bg;
    level.cloud_textures = &texture_groups::blue_cloud;

    level_generator::generate_vertical_level_clouds(level);
    level_generator::generate_level(level, "level24_1.csv");
    level_generator::generate_level(level, "level24_2.csv");

    level_generator::put_q_block_with_coins(level, { 16, 70 }, 10);
    level_generator::put_q_block_with_coins(level, { 14, 9 }, 5);
    level_generator::put_q_block_with_coins(level, { 15, 9 }, 5);

    auto& entities = level.entities;
    entities.red_flying_koopas.push_back(FlyingKoopaState::make_red({ 9.5, 123 }, { 4, 4 }));
    entities.red_flying_koopas.push_back(FlyingKoopaState::make_red({ 9.5, 123 }, { -4, 4 }));
    entities.red_flying_koopas.push_back(FlyingKoopaState::make_red({ 9.5, 116 }, { -4, 0 }));
    entities.red_flying_koopas.push_back(FlyingKoopaState::make_red({ 9.5, 97 }, { 4, 0 }));
    entities.red_flying_koopas.push_back(FlyingKoopaState::make_red({ 9.5, 90 }, { -4, 0 }));
    entities.red_flying_koopas.push_back(FlyingKoopaState::make_red({ 9.5, 82 }, { 4, 0 }));
    entities.red_flying_koopas.push_back(FlyingKoopaState::make_red({ 2.5, 45 }, { 3, 0 }));
    entities.red_flying_koopas.push_back(FlyingKoopaState::make_red({ 2.5, 40 }, { -3, 0 }));
    entities.purple_flying_koopas.push_back(FlyingKoopaState::make_purple({ 16.5, 61 }, { 3, 0 }));
    entities.purple_flying_koopas.push_back(FlyingKoopaState::make_purple({ 9.5, 24 }, { -3, 0 }));
    entities.purple_flying_koopas.push_back(FlyingKoopaState::make_purple({ 9.5, 19 }, { 3, 0 }));

    auto& platforms = level.platforms;
    platforms.push_back(PlatformState({ 8, 146 }, { 0, -30 }));
    platforms.push_back(PlatformState({ 8, 107 }, { 0, -30 }));
    platforms.push_back(PlatformState({ 15, 66 }, { 0, -13 }));
    platforms.push_back(PlatformState({ 1, 50 }, { 0, -18 }));
    platforms.push_back(PlatformState({ 8, 29 }, { 0, -20 }));

    auto& bars = level.fire_bars;
    bars.push_back(FireBarState({ 6, 101 }, 5));
    bars.push_back(FireBarState({ 13, 94 }, 5));
    bars.push_back(FireBarState({ 6, 86 }, 6));
    bars.push_back(FireBarState({ 13, 79 }, 6));
  });
}
