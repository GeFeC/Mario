#pragma once

#include "LevelGenerator/Builder.hpp"
#include "States/AppState.hpp"
#include "States/FireBarState.hpp"
#include "States/LakitoState.hpp"
#include "Views/Level.hpp"
#include "Controllers/LevelController.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"

namespace mario::frames{

static auto run_level61(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level61;
  level.type = LevelState::Type::Horizontal;
  level.number = { 6, 1 };
  level.extra_textures = {
    texture_groups::blue_cloud,
    texture_groups::purple_flying_koopa,
    texture_groups::green_flying_koopa,
    texture_groups::lakito,
    &textures::sky_bg
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::sky_bg;
    level.cloud_textures = &texture_groups::blue_cloud;

    level_generator::generate_horizontal_level_clouds(level, 10);
    level_generator::generate_level(level, "level61_1.csv");
    level_generator::generate_level(level, "level61_2.csv");

    level_generator::put_q_block_with_coins(level, { 159, 9 }, 5);

    level.game_objects.push(LakitoState::make({ 40, 3 }));
    level.game_objects.push(LakitoState::make({ 115, 2 }));
    level.game_objects.push(LakitoState::make({ 130, 4 }));

    for (int i = 0; i < 3; ++i){
      level.game_objects.push(LakitoState::make({ 183, 5 + i * 1.5f }));
    }

    level.game_objects.push(FireBarState({ 84, 10 }, 5));
    level.game_objects.push(FireBarState({ 84, 10 }, 5)).initial_rotation = 180.f;

    level.game_objects.push(FlyingKoopaState::make_green({ 90, 6 }, { 0.f, 5.f }));

    level.game_objects.push(FlyingKoopaState::make_green({ 106, 9 }, { 0.f, 1.f }));
    level.game_objects.push(FlyingKoopaState::make_green({ 110, 8 }, { 0.f, -1.f }));
    level.game_objects.push(FlyingKoopaState::make_green({ 114, 8 }, { 0.f, 1.f }));
    level.game_objects.push(FlyingKoopaState::make_green({ 118, 7 }, { 0.f, -1.f }));

    level.game_objects.push(FlyingKoopaState::make_purple({ 20, 6 }, { 5, 3 }));
    level.game_objects.push(FlyingKoopaState::make_purple({ 20, 6 }, { -5, 3 }));
    level.game_objects.push(FlyingKoopaState::make_purple({ 20, 6 }, { 5, -3 }));
    level.game_objects.push(FlyingKoopaState::make_purple({ 20, 6 }, { -5, -3 }));

    for (int i = 0; i < 3; ++i){
      level.game_objects.push(FlyingKoopaState::make_purple({ 144, 6 + i }, { 0, 3 }));
    }
  });
}

} //namespace mario::frames
