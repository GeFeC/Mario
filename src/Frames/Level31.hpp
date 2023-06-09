#pragma once

#include "LevelGenerator/Builder.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"

namespace mario::frames{

static auto run_level31(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level31;
  level.type = LevelState::Type::Horizontal;
  level.number = { 3, 1 };
  level.extra_textures = {
    texture_groups::blue_cloud,
    texture_groups::purple_flying_koopa,
    &textures::platform,
    &textures::dirt,
    &textures::snow_bg
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::snow_bg;
    level.cloud_textures = &texture_groups::blue_cloud;
    level.player.slip = 1.5f;

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level31_1.csv");
    level_generator::generate_level(level, "level31_2.csv");
    level_generator::generate_level(level, "level31_3.csv");

    auto& objects = level.game_objects;
    auto& platform = objects.push(PlatformState({ 79, 8 }, { 0, -4 }));
    platform.is_active = true;

    objects.push(FlyingKoopaState::make_purple({ 179, 8 }, { 0, 2 }));
  });
}

} //namespace mario::frames
