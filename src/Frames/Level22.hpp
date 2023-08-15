#pragma once

#include "LevelGenerator/Builder.hpp"
#include "LevelGenerator/LevelGenerator.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/FlyingKoopaState.hpp"
#include "res/textureGroups.hpp"

namespace mario::frames{

static auto run_level22(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level22;
  level.type = LevelState::Type::Vertical;
  level.number = { 2, 2 };
  level.extra_textures = {
    texture_groups::blue_cloud,
    texture_groups::red_flying_koopa,
    &textures::bg
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::bg;
    level.cloud_textures = &texture_groups::blue_cloud;

    level_generator::generate_vertical_level_clouds(level);
    level_generator::generate_level(level, "level22_1.csv");
    level_generator::generate_level(level, "level22_2.csv");

    create_hitbox_on_sides(level);

    //Entities
    auto& objects = level.game_objects;
    objects.push(FlyingKoopaState::make_purple({ 10, 71.5 }, { 2, 2 }));
    objects.push(FlyingKoopaState::make_purple({ 7, 40.5 }, { 0, 3 }));
    objects.push(FlyingKoopaState::make_purple({ 4, 25.5 }, { 0, 3 }));

    level.game_objects.get<HammerBroState>(0).jump_state = HammerBroState::JumpState::Down;
    level_generator::put_q_block_with_coins(level, { 2, 103 }, 8);
  });
}

} //namespace mario::frames
