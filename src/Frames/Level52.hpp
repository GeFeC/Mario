#pragma once

#include "LevelGenerator/Builder.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

namespace mario::frames{

static auto run_level52(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level52;
  level.type = LevelState::Type::Vertical;
  level.number = { 5, 2 };
  level.extra_textures = {
    &textures::underground_bg,
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::underground_bg;

    level.player.position = glm::vec2(2, 4) * BlockBase::Size;

    level_generator::generate_level(level, "level52_1.csv");
    level_generator::generate_level(level, "level52_2.csv");

    level.game_objects.push(FireBarState({ 7, 43 }, 4));
  });
}

} //namespace mario::frames
