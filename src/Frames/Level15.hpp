#pragma once

#include "States/AppState.hpp"
#include "Views/Level.hpp"
#include "Controllers/LevelController.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

static auto run_frame_level15(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level15;
  level.type = LevelState::Type::Vertical;
  level.number = { 1, 5 };
  level.extra_textures = {
    texture_groups::red_cloud,
    texture_groups::red_flying_koopa,
    texture_groups::green_flying_koopa,
    &textures::mushroom_bg
  };

  run_frame_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::mushroom_bg;
    level.cloud_textures = &texture_groups::red_cloud;

    level.fire_bars.push_back(FireBarState({ 7, 102 }, 3));
    level.fire_bars.push_back(FireBarState({ 10, 102 }, 3));
    level.fire_bars.push_back(FireBarState({ 13, 102 }, 3));

    level.fire_bars.push_back(FireBarState({ 9, 76 }, 6));
    level.fire_bars.push_back(FireBarState({ 9, 83 }, 6));

    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 10, 98 }, { 0, 3 }));

    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 14, 67 }, { 3, 0 }));

    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 8, 37 }, { 0, 3 }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 8, 37 }, { 0, -3 }));

    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 10, 26 }, { 0, 3 }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 10, 26 }, { 0, -3 }));

    const auto r = 8;
    const auto r2 = r / 1.414f;
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 13, 13 }, { 0, r }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 13, 13 }, { 0, -r }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 13, 13 }, { r, 0 }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 13, 13 }, { -r, 0 }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 13, 13 }, { r2, r2 }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 13, 13 }, { -r2, r2 }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 13, 13 }, { r2, -r2 }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 13, 13 }, { -r2, -r2 }));

    level_generator::put_q_block_with_coins(level, { 13, 13 }, 10);

    level_generator::generate_vertical_level_clouds(level);
    level_generator::generate_level(level, "level15_1.csv");
    level_generator::generate_level(level, "level15_2.csv");
  });
}
