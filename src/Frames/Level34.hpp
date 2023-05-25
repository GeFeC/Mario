#pragma once

#include "LevelGenerator/Builder.hpp"

#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/EntityPusherState.hpp"
#include "States/FlyingKoopaState.hpp"
#include "res/textureGroups.hpp"

static auto run_frame_level34(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level34;
  level.type = LevelState::Type::Horizontal;
  level.number = { 3, 4 };
  level.extra_textures = {
    texture_groups::blue_cloud,
    &textures::platform,
    &textures::snow_bg
  };

  run_frame_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::snow_bg;
    level.cloud_textures = &texture_groups::blue_cloud;
    level.player.slip = 1.5f;
    level.player.position = glm::vec2(95, 7) * BlockBase::Size;

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level34_1.csv");
    level_generator::generate_level(level, "level34_2.csv");

    auto& objects = level.game_objects;
    objects.push(PlatformState({ 97, 10 }, { 12, -6 }));

    objects.push(FlyingKoopaState::make_red({ 48, 9 }, { 3, 0 }));

    objects.push(PlantState::make_red({ 41.5, 2.5 }))
    .vertical_flip = Drawable::Flip::UseFlip;
    objects.push(PlantState::make_red({ 88.5, 0.5 }))
    .vertical_flip = Drawable::Flip::UseFlip;
    objects.push(PlantState::make_red({ 102.5, 3.5 }))
    .vertical_flip = Drawable::Flip::UseFlip;
    objects.push(PlantState::make_red({ 104.5, 2.5 }))
    .vertical_flip = Drawable::Flip::UseFlip;
    objects.push(PlantState::make_red({ 106.5, 1.5 }))
    .vertical_flip = Drawable::Flip::UseFlip;
    objects.push(PlantState::make_red({ 108.5, 0.5 }))
    .vertical_flip = Drawable::Flip::UseFlip;
    objects.push(PlantState::make_red({ 110.5, -0.5 }))
    .vertical_flip = Drawable::Flip::UseFlip;

    for (int i = 0; i < 10; ++i){
      const auto sign = i % 2 ? 1 : -1;
      objects.push(FlyingKoopaState::make_red({ 123 + i, 5 }, { 0, 4 * sign }));
    }
  });
}
