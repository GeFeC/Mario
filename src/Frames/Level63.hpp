#pragma once

#include "LevelGenerator/Builder.hpp"
#include "States/AppState.hpp"
#include "States/FireBarState.hpp"
#include "States/FlyingKoopaState.hpp"
#include "States/LakitoState.hpp"
#include "Views/Level.hpp"
#include "Controllers/LevelController.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"

namespace mario::frames{

static auto run_level63(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level63;
  level.type = LevelState::Type::Horizontal;
  level.number = { 6, 3 };
  level.extra_textures = {
    texture_groups::blue_cloud,
    texture_groups::lakito,
    texture_groups::spike,
    &textures::sky_bg
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::sky_bg;
    level.cloud_textures = &texture_groups::blue_cloud;

    level.game_objects.push(LakitoState::make({ 21, 1 }));
    level.game_objects.push(FireBarState({ 52, 7 }, 5));
    level.game_objects.push(FireBarState({ 58, 8 }, 5));
    level.game_objects.push(FlyingKoopaState::make_purple({ 168, 5 }, { 0.f, 4.f }));
    level.game_objects.push(FlyingKoopaState::make_purple({ 168, 5 }, { 0.f, -4.f }));

    level_generator::generate_horizontal_level_clouds(level, 10);
    level_generator::generate_level(level, "level63_1.csv");
    level_generator::generate_level(level, "level63_2.csv");
  });
}

} //namespace mario::frames
