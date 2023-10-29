#pragma once

#include "LevelGenerator/Builder.hpp"
#include "LevelGenerator/LevelGenerator.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/FireBarState.hpp"
#include "States/FlyingKoopaState.hpp"
#include "States/HammerBroState.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

namespace mario::frames{

static auto run_level65(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level65;
  level.type = LevelState::Type::Vertical;
  level.number = { 6, 5 };
  level.extra_textures = {
    &textures::sky_bg,
    &textures::platform,
    texture_groups::blue_cloud,
    texture_groups::lakito,
    texture_groups::yellow_goomba,
    texture_groups::purple_flying_koopa,
    texture_groups::red_hammerbro
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::sky_bg;
    level.cloud_textures = &texture_groups::blue_cloud;

    auto& objects = level.game_objects;
    for (int i = 0; i < 4; ++i){
      objects.push(FireBarState({ 5, 138 }, 3)).initial_rotation = 90.f * i;
      objects.push(FireBarState({ 9, 138 }, 3)).initial_rotation = 90.f * i;
      objects.push(FireBarState({ 13, 138 }, 3)).initial_rotation = 90.f * i;
      objects.push(FireBarState({ 17, 138 }, 3)).initial_rotation = 90.f * i;

      objects.push(FireBarState({ 11, 102 }, 3)).initial_rotation = 90.f * i;
    }

    for (int i = -1; i <= 1; ++i){
      objects.push(FlyingKoopaState::make_purple({ 11, 132 }, { -3, i * 2.5 }));
    }

    for (int i = 2; i <= 16; ++i){
      auto& spike = objects.push(SpikeState::make({ i, 128 }));
      spike.can_move = false;
      spike.flip_gravity();
    }

    objects.push(PlatformState({ 16, 140 }, { -14.f, 0.f }));
    objects.push(PlatformState({ 3, 129 }, { 10.f, 0.f }));

    objects.push(LakitoState::make({ 9.5, 141 }));

    for (int i = 86; i < 94; i += 4){
      objects.push(LakitoState::make({ 0, i - 0.5 }));
    }

    for (int i = 3; i <= 11; i += 8){
      objects.push(PlatformState({ i, 73 }, { 0.f, 5 }, 6))
        .is_active = true;
      objects.push(GoombaState::make_yellow({ i, 74 }))
        .flip_gravity();
    }

    for (int i = 7; i <= 15; i += 8){
      objects.push(PlatformState({ i, 78 }, { 0.f, -5 }, 6))
        .is_active = true;
      objects.push(GoombaState::make_yellow({ i + 1, 78.5 }))
        .flip_gravity();
    }

    for (int i = 3; i <= 17; ++i){
      auto& flipped_spike = objects.push(SpikeState::make({ i, 72 }));
      flipped_spike.can_move = false;
      flipped_spike.flip_gravity();

      objects.push(SpikeState::make({ i, 79 }))
        .can_move = false;
    }

    objects.push(PlantState::make_red({ 14, 102.5 }))
      .vertical_flip = util::Flip::flip();
    objects.push(PlantState::make_red({ 7, 102.5 }))
      .vertical_flip = util::Flip::flip();

    level_generator::put_q_block_with_entity(level, HammerBroState::make_red({ 17, 67 }));

    level_generator::generate_vertical_level_clouds(level);
    level_generator::generate_level(level, "level65_1.csv");
    level_generator::generate_level(level, "level65_2.csv");
  });
}

} //namespace mario::frames
