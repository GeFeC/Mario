#pragma once

#include "Controllers/BossController.hpp"
#include "LevelGenerator/LevelGenerator.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/BossState.hpp"
#include "res/textureGroups.hpp"

namespace mario::frames{

static auto run_level36(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level36;
  level.type = LevelState::Type::Boss;
  level.number = { 3, 6 };
  level.extra_textures = {
    texture_groups::beetle,
    texture_groups::blue_cloud,
    &textures::snow_bg
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::snow_bg;
    level.cloud_textures = &texture_groups::blue_cloud;
    level.player.slip = 1.5f;

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level36_1.csv");
    level_generator::generate_level(level, "level36_2.csv");

    auto& objects = level.game_objects;
    auto& boss = objects.push(KingBeetleState());

    objects.push(BeetleState::make({ 2.5f, 13.f }));

    boss.position = glm::vec2(16, 6) * BlockBase::Size;

    level.stats.boss_hp = &boss.hp;
    level.stats.max_boss_hp = KingBeetleState::MaxHp;

    level.player.growth = PlayerState::Growth::Big;
    level.player.form = PlayerState::Form::Fire;
    level.player.position.x = 2 * BlockBase::Size;
  });
}

} //namespace mario::frames
