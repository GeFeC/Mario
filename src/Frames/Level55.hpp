#pragma once

#include "LevelGenerator/Builder.hpp"
#include "States/AppState.hpp"
#include "States/BlackPlantState.hpp"
#include "States/HammerBroState.hpp"
#include "Views/Level.hpp"
#include "Controllers/LevelController.hpp"
#include "Frame.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"
#include "Frames/LevelBase.hpp"

namespace mario::frames{

static auto run_level55(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level55;
  level.type = LevelState::Type::Horizontal;
  level.number = { 5, 5 };
  level.extra_textures = {
    &textures::underground_bg,
    texture_groups::q_block
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::underground_bg;

    for (int i = 0; i < 9; ++i){
      level.game_objects.push(PlantState::make_red({ 4 * i, -0.5f }))
      .vertical_flip = util::Flip::flip();
    }

    for (int i = 0; i < 7; ++i){
      level.game_objects.push(PlantState::make_red({ 62 + 4 * i, -0.5f }))
      .vertical_flip = util::Flip::flip();
    }

    level.game_objects.push(BlackPlantState::make({ 44, -0.5f }))
    .vertical_flip = util::Flip::flip();

    level.game_objects.push(BlackPlantState::make({ 55, -0.5f }))
    .vertical_flip = util::Flip::flip();

    level.game_objects.push(FireBarState({ 112, 7 }, 4));
    level.game_objects.push(FireBarState({ 112, 7 }, 4))
    .initial_rotation = 180.f;

    level.game_objects.push(FireBarState({ 120, 6 }, 4))
    .initial_rotation = 90.f;
    level.game_objects.push(FireBarState({ 120, 6 }, 4))
    .initial_rotation = 270.f;

    for (int i = 0; i < 4; ++i){
      level.game_objects.push(FireBarState({ 125, 3 }, 4))
      .initial_rotation = 90.f * i;
    }

    for (int i = 0; i < 3; ++i){
      level.game_objects.push(FireBarState({ 185, 7 }, 4))
      .initial_rotation = 120.f * i;
    }

    for (int i = 0; i < 9; ++i){
      level.game_objects.push(BlackPlantState::make({ 141 + i * 4, -0.5f }))
      .vertical_flip = util::Flip::flip();
    }

    level.game_objects.push(PlantState::make_red({ 192, -0.5f }))
    .vertical_flip = util::Flip::flip();

    level.game_objects.push(PlantState::make_red({ 196, -0.5f }))
    .vertical_flip = util::Flip::flip();

    level_generator::put_q_block_with_entity(level, HammerBroState::make({ 133, 6 }));

    level_generator::generate_level(level, "level55_1.csv");
    level_generator::generate_level(level, "level55_2.csv");
  });
}

} //namespace mario::frames
