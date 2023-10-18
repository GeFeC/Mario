#pragma once

#include "LevelGenerator/Builder.hpp"
#include "States/AppState.hpp"
#include "States/FireBarState.hpp"
#include "States/FlyingKoopaState.hpp"
#include "States/LakitoState.hpp"
#include "Views/Level.hpp"
#include "Controllers/LevelController.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"

namespace mario::frames{

static auto run_level64(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level64;
  level.type = LevelState::Type::Horizontal;
  level.number = { 6, 4 };
  level.extra_textures = {
    texture_groups::blue_cloud,
    texture_groups::lakito,
    texture_groups::spike,
    texture_groups::yellow_goomba,
    texture_groups::purple_flying_koopa,
    texture_groups::green_flying_koopa,
    &textures::sky_bg
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::sky_bg;
    level.cloud_textures = &texture_groups::blue_cloud;

    auto& objects = level.game_objects;
    objects.push(SpikeState::make({ 101, 5 })).flip_gravity();
    objects.push(JumpingKoopaState::make_purple({ 105, 5.5 })).flip_gravity();
    objects.push(JumpingKoopaState::make_purple({ 109, 5.5 })).flip_gravity();
    objects.push(GoombaState::make_yellow({ 29, 3 })).flip_gravity();
    objects.push(KoopaState::make_purple({ 75, 8.5 })).flip_gravity();
    objects.push(FireBarState({ 38, 4 }, 5));
    objects.push(FireBarState({ 38, 4 }, 5)).initial_rotation = 180.f;

    objects.push(FireBarState({ 73, 3 }, 7));

    objects.push(FlyingKoopaState::make_green({ 64, 2 }, { 2, 0 }));
    objects.push(FlyingKoopaState::make_green({ 83, 7 }, { 2, 0 }));
    objects.push(KoopaState::make_red({ 40, 5.5 })).flip_gravity();

    level_generator::generate_horizontal_level_clouds(level, 10);
    level_generator::generate_level(level, "level64_1.csv");
    level_generator::generate_level(level, "level64_2.csv");
  });
}

} //namespace mario::frames
