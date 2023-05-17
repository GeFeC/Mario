#pragma once

#include "LevelGenerator/Builder.hpp"
#include "LevelGenerator/LevelGenerator.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/FlyingKoopaState.hpp"
#include "Util/Util.hpp"
#include "res/textureGroups.hpp"

static auto run_frame_level25(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level25;
  level.type = LevelState::Type::Horizontal;
  level.number = { 2, 5 };
  level.extra_textures = {
    texture_groups::blue_cloud,
    texture_groups::green_flying_koopa,
    &textures::platform,
    &textures::bg
  };

  run_frame_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::bg;
    level.cloud_textures = &texture_groups::blue_cloud;

    level_generator::generate_horizontal_level_clouds(level, 10);
    level_generator::generate_level(level, "level25_1.csv");
    level_generator::generate_level(level, "level25_2.csv");

    level_generator::put_q_block_with_coins(level, { 59, 1 }, 8);

    auto& objects = level.game_objects;
    objects.push(FireBarState({ 76, 4 }, 4));
    objects.push(FireBarState({ 146, 5 }, 4));
    objects.push(FireBarState({ 154, 5 }, 4));

    objects.push(PlatformState({ 134, 8 }, { 0, -5 }, 10))
    .is_active = true;
    objects.push(PlatformState({ 144, 1 }, { 0, 9 }, 10))
    .is_active = true;
    objects.push(PlatformState({ 153, 9 }, { 0, -7 }, 6))
    .is_active = true;
    objects.push(PlatformState({ 186, 9 }, { 0, -5 }, 6))
    .is_active = true;

    objects.push(LoopedPlatformState({ 89, 0 }, { 0, -1 }));
    objects.push(LoopedPlatformState({ 89, 6 }, { 0, -1 }));

    objects.push(LoopedPlatformState({ 97, 0 }, { 0, 1 }));
    objects.push(LoopedPlatformState({ 97, 6 }, { 0, 1 }));

    objects.push(FlyingKoopaState::make_green({ 194, 4 }, { 0.5, 0 }));
  });
}
