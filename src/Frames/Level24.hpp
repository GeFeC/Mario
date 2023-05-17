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

    auto& objects = level.game_objects;
    objects.push(FlyingKoopaState::make_red({ 9.5, 123 }, { 4, 4 }));
    objects.push(FlyingKoopaState::make_red({ 9.5, 123 }, { -4, 4 }));
    objects.push(FlyingKoopaState::make_red({ 9.5, 116 }, { -4, 0 }));
    objects.push(FlyingKoopaState::make_red({ 9.5, 97 }, { 4, 0 }));
    objects.push(FlyingKoopaState::make_red({ 9.5, 90 }, { -4, 0 }));
    objects.push(FlyingKoopaState::make_red({ 9.5, 82 }, { 4, 0 }));
    objects.push(FlyingKoopaState::make_red({ 2.5, 45 }, { 3, 0 }));
    objects.push(FlyingKoopaState::make_red({ 2.5, 40 }, { -3, 0 }));

    objects.push(FlyingKoopaState::make_purple({ 16.5, 61 }, { 3, 0 }));
    objects.push(FlyingKoopaState::make_purple({ 9.5, 24 }, { -3, 0 }));
    objects.push(FlyingKoopaState::make_purple({ 9.5, 19 }, { 3, 0 }));

    objects.push(PlatformState({ 8, 146 }, { 0, -30 }));
    objects.push(PlatformState({ 8, 107 }, { 0, -30 }));
    objects.push(PlatformState({ 15, 66 }, { 0, -13 }));
    objects.push(PlatformState({ 1, 50 }, { 0, -18 }));
    objects.push(PlatformState({ 8, 29 }, { 0, -20 }));

    objects.push(FireBarState({ 6, 101 }, 5));
    objects.push(FireBarState({ 13, 94 }, 5));
    objects.push(FireBarState({ 6, 86 }, 6));
    objects.push(FireBarState({ 13, 79 }, 6));
  });
}
