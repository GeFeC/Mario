#pragma once

#include "LevelGenerator/LevelGenerator.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "Util/Util.hpp"
#include "res/textureGroups.hpp"
#include <thread>

static auto run_frame_level23(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level23;
  level.type = LevelState::Type::Horizontal;
  level.number = { 2, 3 };
  level.extra_textures = {
    texture_groups::blue_cloud,
    texture_groups::red_flying_koopa,
    texture_groups::purple_flying_koopa,
    &textures::platform,
    &textures::bg
  };

  run_frame_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::bg;
    level.cloud_textures = &texture_groups::blue_cloud;

    level_generator::generate_horizontal_level_clouds(level, 10);
    level_generator::generate_level(level, "level23_1.csv");
    level_generator::generate_level(level, "level23_2.csv");

    level.platforms.push_back(PlatformState({ 26, 10 }, { 10, 0 }));
    level.platforms.push_back(PlatformState({ 55, 6 }, { 10, 0 }));
    level.platforms.push_back(PlatformState({ 88, 4 }, { 0, 7 }));
    level.platforms.back().is_active = true;
    level.platforms.push_back(PlatformState({ 95, 4 }, { 0, 7 }));
    level.platforms.back().is_active = true;
    level.platforms.back().timer = glm::radians(180.f);
    level.platforms.push_back(PlatformState({ 102, 4 }, { 0, 7 }));
    level.platforms.back().is_active = true;
    level.platforms.back().timer = glm::radians(90.f);
    level.platforms.push_back(PlatformState({ 125, 10 }, { 0, -7 }, 20));
    level.platforms.back().is_active = true;
    level.platforms.push_back(PlatformState({ 140, 3 }, { 0, 7 }, 20));
    level.platforms.back().is_active = true;
    level.platforms.push_back(PlatformState({ 172, 6 }, { 5, 0 }));
    level.platforms.back().is_active = true;
    level.platforms.push_back(PlatformState({ 189, 6 }, { -5, 0 }));
    level.platforms.back().is_active = true;

    level.entities.red_flying_koopas.push_back(FlyingKoopaState::make_red( { 32, 8 }, { 2, 3 } ));
    level.entities.red_flying_koopas.push_back(FlyingKoopaState::make_red( { 130, 5 }, { 0, 5 } ));
    level.entities.red_flying_koopas.push_back(FlyingKoopaState::make_red( { 137, 5 }, { 0, -5 } ));
    level.entities.red_flying_koopas.push_back(FlyingKoopaState::make_red( { 145, 5 }, { 0, 5 } ));
    level.entities.purple_flying_koopas.push_back(FlyingKoopaState::make_purple( { 63, 4 }, { 3, 0 } ));
    level.entities.purple_flying_koopas.push_back(FlyingKoopaState::make_purple( { 96.5, 6 }, { 4, 4 } ));
    level.entities.purple_flying_koopas.push_back(FlyingKoopaState::make_purple( { 96.5, 6 }, { -4, 4 } ));

    level.fire_bars.push_back(FireBarState({ 163, 6 }, 4));
  });
}
