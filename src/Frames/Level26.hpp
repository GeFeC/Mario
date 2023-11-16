#pragma once

#include "Controllers/BossController.hpp"
#include "LevelGenerator/LevelGenerator.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/BossState.hpp"
#include "res/textureGroups.hpp"

namespace mario::frames{

static auto run_level26(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level26;
  level.type = LevelState::Type::Boss;
  level.number = { 2, 6 };
  level.extra_textures = {
    texture_groups::green_koopa,
    texture_groups::blue_cloud,
    &textures::bg
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::bg;
    level.cloud_textures = &texture_groups::blue_cloud;

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level26_1.csv");
    level_generator::generate_level(level, "level26_2.csv");

    auto& objects = level.game_objects;
    objects.push(FireBarState({ 0, 7 }, 4));
    objects.push(FireBarState({ 19, 7 }, 4));

    auto& boss = objects.push(KingKoopaState());
    boss.position = glm::vec2(16, 6) * BlockBase::Size;

    level.stats.boss_hp = { &boss.hp, KingKoopaState::MaxHp };

    level.player.set_form_to_fire();
  });
}

} //namespace mario::frames
