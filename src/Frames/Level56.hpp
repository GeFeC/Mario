#pragma once

#include "LevelGenerator/LevelGenerator.hpp"
#include "States/BossState.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

namespace mario::frames{

static auto run_level56(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level56;
  level.type = LevelState::Type::Boss;
  level.number = { 5, 6 };
  level.extra_textures = {
    &textures::underground_bg,
    texture_groups::black_plant
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::underground_bg;

    level_generator::generate_level(level, "level56_1.csv");
    level_generator::generate_level(level, "level56_2.csv");

    auto& objects = level.game_objects;
    objects.push(FireBarState({ 0, 11 }, 5));
    objects.push(FireBarState({ 19, 11 }, 5));

    auto& boss = objects.push(KingPlantState());
    boss.position = glm::vec2(14.5f, 11.2f) * BlockBase::Size;

    level.stats.boss_hp = { &boss.hp, KingPlantState::MaxHp };
    level.player.set_form_to_fire();
    level.player.position = glm::vec2(9.5f, 1.f) * BlockBase::Size;
  });
}

} //namespace mario::frames
