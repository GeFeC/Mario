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

    level.fire_bars.push_back(FireBarState({ 7, 152 }, 3));
    level.fire_bars.push_back(FireBarState({ 10, 152 }, 3));
    level.fire_bars.push_back(FireBarState({ 13, 152 }, 3));

    level.fire_bars.push_back(FireBarState({ 9, 126 }, 6));
    level.fire_bars.push_back(FireBarState({ 9, 133 }, 6));

    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 10, 148 }, { 0, 3 }));

    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 14, 117 }, { 3, 0 }));

    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 8, 87 }, { 0, 3 }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 8, 87 }, { 0, -3 }));

    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 10, 76 }, { 0, 3 }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 10, 76 }, { 0, -3 }));

    const auto r = 8;
    const auto r2 = r / 1.414f;
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 13, 63 }, { 0, r }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 13, 63 }, { 0, -r }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 13, 63 }, { r, 0 }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 13, 63 }, { -r, 0 }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 13, 63 }, { r2, r2 }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 13, 63 }, { -r2, r2 }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 13, 63 }, { r2, -r2 }));
    level.entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 13, 63 }, { -r2, -r2 }));

    level_generator::put_qblock_with_coins(level, { 13, 63 }, 10);

    level_generator::generate_vertical_level_clouds(level);
    level_generator::generate_level(level, "level15_1.csv");
    level_generator::generate_level(level, "level15_2.csv");
  });
}
