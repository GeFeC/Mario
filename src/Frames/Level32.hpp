#pragma once

#include "LevelGenerator/Builder.hpp"

#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/EntityPusherState.hpp"
#include "States/FlyingKoopaState.hpp"
#include "res/textureGroups.hpp"

static auto run_frame_level32(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level32;
  level.type = LevelState::Type::Horizontal;
  level.number = { 3, 2 };
  level.extra_textures = {
    texture_groups::blue_cloud,
    texture_groups::green_flying_koopa,
    texture_groups::red_flying_koopa,
    texture_groups::yellow_goomba,
    texture_groups::purple_koopa,
    texture_groups::purple_flying_koopa,
    &textures::platform,
    &textures::dirt,
    &textures::snow_bg
  };

  run_frame_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::snow_bg;
    level.cloud_textures = &texture_groups::blue_cloud;
    level.player.slip = 1.5f;
    level.player.position = glm::vec2(172, 3) * BlockBase::Size;

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level32_1.csv");
    level_generator::generate_level(level, "level32_2.csv");
    level_generator::generate_level(level, "level32_3.csv");

    level_generator::put_q_block_with_entity(level, GoombaState::make_normal({ 11, 7 }));
    level_generator::put_q_block_with_entity(level, GoombaState::make_yellow({ 78, 7 }));
    level_generator::put_q_block_with_entity(level, GoombaState::make_yellow({ 83, 7 }));

    level_generator::put_q_block_with_entity(level, KoopaState::make_purple({ 80, 7 }));
    level_generator::put_q_block_with_entity(level, KoopaState::make_purple({ 85, 7 }));

    level_generator::put_q_block_with_entity(level, FlyingKoopaState::make_red({ 148, 8 }, { 0, 1 }))
    .pusher.push_distance_multiplier = 5.f;
    level_generator::put_q_block_with_entity(level, FlyingKoopaState::make_red({ 151, 8 }, { 0, 1 }))
    .pusher.push_distance_multiplier = 5.f;
    level_generator::put_q_block_with_entity(level, FlyingKoopaState::make_red({ 154, 8 }, { 0, 1 }))
    .pusher.push_distance_multiplier = 5.f;
    level_generator::put_q_block_with_entity(level, FlyingKoopaState::make_red({ 157, 8 }, { 0, 1 }))
    .pusher.push_distance_multiplier = 5.f;
    level_generator::put_q_block_with_entity(level, FlyingKoopaState::make_red({ 160, 8 }, { 0, 1 }))
    .pusher.push_distance_multiplier = 5.f;
    level_generator::put_q_block_with_entity(level, FlyingKoopaState::make_red({ 163, 8 }, { 0, 1 }))
    .pusher.push_distance_multiplier = 5.f;
    level_generator::put_q_block_with_entity(level, FlyingKoopaState::make_red({ 166, 8 }, { 0, 1 }))
    .pusher.push_distance_multiplier = 5.f;

    level_generator::put_q_block_with_entity(level, JumpingKoopaState::make_green({ 63, 7 }));

    auto& objects = level.game_objects;
    objects.push(FlyingKoopaState::make_purple({ 102, 6 }, { 0, 5 }));

    objects.push(FireBarState({ 90, 4 }, 5));
    objects.push(FireBarState({ 115, 9 }, 5));
    objects.push(FireBarState({ 120, 4 }, 5));
    objects.push(FireBarState({ 113, 2 }, 5));

    objects.push(PlatformState({ 110, 9 }, { 7, -7 }));
    
  });
}
