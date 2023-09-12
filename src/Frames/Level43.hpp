#pragma once

#include "LevelGenerator/Builder.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/BlockState.hpp"
#include "States/FireBarState.hpp"
#include "res/textureGroups.hpp"

namespace mario::frames{

static auto run_level43(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level43;
  level.type = LevelState::Type::Vertical;
  level.number = { 4, 3 };
  level.extra_textures = {
    texture_groups::blue_cloud,
    &textures::bg,
    &textures::water_top,
    &textures::water_bottom,
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::bg;
    level.cloud_textures = &texture_groups::blue_cloud;
    level.water_level = 1.f;

    level.game_objects.push(FishState::make_red({ 4, 111 }))
      .set_direction(util::Direction::right());

    level.game_objects.push(FishState::make_red({ 6, 115 }))
      .set_direction(util::Direction::right());


    level.game_objects.push(FishState::make_red({ 5, 76 }))
      .set_direction(util::Direction::right());

    level.game_objects.push(FishState::make_red({ 10, 81 }))
      .set_direction(util::Direction::right());

    level.game_objects.push(FishState::make_red({ 2, 87 }))
      .set_direction(util::Direction::right());

    level.game_objects.push(FishState::make_red({ 6, 97 }))
      .set_direction(util::Direction::right());


    level.game_objects.push(FishState::make_grey({ 9, 112 }))
      .set_direction(util::Direction::right());

    level.game_objects.push(FishState::make_grey({ 7, 117 }))
      .set_direction(util::Direction::right());

    level.game_objects.push(FishState::make_grey({ 2, 82 }))
      .set_direction(util::Direction::right());

    level.game_objects.push(FishState::make_grey({ 6, 84 }))
      .set_direction(util::Direction::right());

    level.game_objects.push(FishState::make_grey({ 4, 90 }))
      .set_direction(util::Direction::right());

    level.game_objects.push(FishState::make_grey({ 9, 96 }))
      .set_direction(util::Direction::right());

    level_generator::generate_level(level, "level43_1.csv");
    level_generator::generate_level(level, "level43_2.csv");
  });
}

} //namespace mario::frames
