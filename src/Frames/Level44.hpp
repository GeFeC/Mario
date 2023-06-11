#pragma once

#include "LevelGenerator/Builder.hpp"
#include "LevelGenerator/LevelGenerator.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"

namespace mario::frames{

static auto run_level44(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level44;
  level.type = LevelState::Type::Horizontal;
  level.number = { 4, 4 };
  level.extra_textures = {
    texture_groups::blue_cloud,
    texture_groups::red_fish,
    texture_groups::red_flying_koopa,
    &textures::dirt,
    &textures::bg,
    &textures::water_top,
    &textures::water_bottom,
    &textures::platform
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::bg;
    level.cloud_textures = &texture_groups::blue_cloud;
    level.water_level = 7.f;

    const auto level_start_x = BlockBase::Size;
    if (level.current_checkpoint.x == level_start_x){
      level.player.position.y = BlockBase::Size * 5.f;
    }

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level44_1.csv");
    level_generator::generate_level(level, "level44_2.csv");

    level.game_objects.push(PlatformState({ 76, 5 }, { 20, -3 }));

    for (int i = 106; i < 124; i += FishState::random_value(3, 5)){
      level.game_objects.push(FishState::make_jumping({ i, 12.f }));
    }

    level.game_objects.push(FlyingKoopaState::make_red({ 130, 4 }, { 0.f, 2.f }));
    level.game_objects.push(FlyingKoopaState::make_red({ 136, 4 }, { 0.f, 2.f }));
  });
}

}
