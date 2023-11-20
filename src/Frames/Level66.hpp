#pragma once

#include "LevelGenerator/LevelGenerator.hpp"
#include "States/BossState.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

namespace mario::frames{

static auto run_level66(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level66;
  level.type = LevelState::Type::Boss;
  level.number = { 6, 6 };
  level.extra_textures = {
    &textures::sky_bg,
    texture_groups::blue_cloud,
    texture_groups::lakito,
    texture_groups::spike
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::sky_bg;
    level.cloud_textures = &texture_groups::blue_cloud;

    level_generator::generate_horizontal_level_clouds(level, 10);
    level_generator::generate_level(level, "level66_1.csv");
    level_generator::generate_level(level, "level66_2.csv");

    auto& objects = level.game_objects;
    auto& boss = objects.push(KingLakitoState());

    level.stats.boss_hp = { &boss.hp, KingLakitoState::MaxHp };

    level.player.set_form_to_fire();
    level.player.position = glm::vec2(4.f, 1.f) * BlockBase::Size;
    level.player.flip_gravity();
  });
}

} //namespace mario::frames
