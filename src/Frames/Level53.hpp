#pragma once

#include "States/AppState.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"
#include "Frames/LevelBase.hpp"

namespace mario::frames{

static auto run_level53(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level53;
  level.type = LevelState::Type::Horizontal;
  level.number = { 5, 3 };
  level.extra_textures = {
    &textures::underground_bg,
    &textures::darkness_view,
    &textures::platform,
    texture_groups::red_flying_koopa,
    texture_groups::purple_flying_koopa
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::underground_bg;
    level.is_dark = true;

    level_generator::generate_level(level, "level53_1.csv");
    level_generator::generate_level(level, "level53_2.csv");

    level.game_objects.push(FlyingKoopaState::make_purple({ 38, 5 }, { 0, 1 }));
    level.game_objects.push(PlatformState({ 115, 8 }, { 20, 0 }));

    level.game_objects.push(FlyingKoopaState::make_red({ 120, 5 }, { 0, 1.5f }));
    level.game_objects.push(FlyingKoopaState::make_red({ 126, 5 }, { 0, -1.5f }));
    level.game_objects.push(FlyingKoopaState::make_red({ 132, 5 }, { 0, 1.5f }));
  });
}

} //namespace mario::frames
