#pragma once

#include "LevelGenerator/LevelGenerator.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "Util/Util.hpp"
#include "res/textureGroups.hpp"

namespace mario::frames{

static auto run_level23(AppState& app){
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

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::bg;
    level.cloud_textures = &texture_groups::blue_cloud;

    level_generator::generate_horizontal_level_clouds(level, 10);
    level_generator::generate_level(level, "level23_1.csv");
    level_generator::generate_level(level, "level23_2.csv");

    auto& objects = level.game_objects;
    objects.push(PlatformState({ 26, 10 }, { 10, 0 }));
    objects.push(PlatformState({ 55, 6 }, { 10, 0 }));
    objects.push(PlatformState({ 88, 4 }, { 0, 7 }));
    objects.back<PlatformState>().is_active = true;

    objects.push(PlatformState({ 95, 4 }, { 0, 7 }));
    objects.back<PlatformState>().is_active = true;
    objects.back<PlatformState>().timer = glm::radians(180.f);

    objects.push(PlatformState({ 102, 4 }, { 0, 7 }));
    objects.back<PlatformState>().is_active = true;
    objects.back<PlatformState>().timer = glm::radians(90.f);

    objects.push(PlatformState({ 125, 10 }, { 0, -7 }, 20));
    objects.back<PlatformState>().is_active = true;

    objects.push(PlatformState({ 140, 3 }, { 0, 7 }, 20));
    objects.back<PlatformState>().is_active = true;

    objects.push(PlatformState({ 172, 6 }, { 5, 0 }));
    objects.back<PlatformState>().is_active = true;

    objects.push(PlatformState({ 189, 6 }, { -5, 0 }));
    objects.back<PlatformState>().is_active = true;

    objects.push(FlyingKoopaState::make_red( { 32, 8 }, { 2, 3 } ));
    objects.push(FlyingKoopaState::make_red( { 130, 5 }, { 0, 5 } ));
    objects.push(FlyingKoopaState::make_red( { 137, 5 }, { 0, -5 } ));
    objects.push(FlyingKoopaState::make_red( { 145, 5 }, { 0, 5 } ));
    objects.push(FlyingKoopaState::make_purple( { 63, 4 }, { 3, 0 } ));
    objects.push(FlyingKoopaState::make_purple( { 96.5, 6 }, { 4, 4 } ));
    objects.push(FlyingKoopaState::make_purple( { 96.5, 6 }, { -4, 4 } ));

    auto& fire_bars = level.game_objects.get_vec<FireBarState>();
    fire_bars.push_back(FireBarState({ 163, 6 }, 4));
  });
}

} //namespace mario::frames
