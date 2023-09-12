#pragma once

#include "LevelGenerator/Builder.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/BlockState.hpp"
#include "States/FireBarState.hpp"
#include "States/HammerBroState.hpp"
#include "res/textureGroups.hpp"

namespace mario::frames{

static auto run_level45(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level45;
  level.type = LevelState::Type::Vertical;
  level.number = { 4, 5 };
  level.extra_textures = {
    texture_groups::blue_cloud,
    texture_groups::red_hammerbro,
    &textures::platform,
    &textures::bg,
    &textures::water_top,
    &textures::water_bottom,
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::bg;
    level.cloud_textures = &texture_groups::blue_cloud;
    level.water_level = 9.f;

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level45_1.csv");
    level_generator::generate_level(level, "level45_2.csv");

    level_generator::put_q_block_with_entity(level, HammerBroState::make_red({ 4, 4 }));

    level.game_objects.push(PlatformState({ 2, 122 }, { 8, 0 }, 16))
    .is_active = true;
    level.game_objects.push(PlatformState({ 10, 132 }, { -8, 0 }, 16))
    .is_active = true;
    level.game_objects.push(PlatformState({ 10, 112 }, { -8, 0 }, 16))
    .is_active = true;

    level.game_objects.push(FishState::make_red({ 7, 97 }))
      .set_direction(util::Direction::right());

    level.game_objects.push(FishState::make_red({ 5, 100 }))
      .set_direction(util::Direction::right());

    level.game_objects.push(FishState::make_red({ 7, 105 }))
      .set_direction(util::Direction::right());

  });
}

} //namespace mario::frames
