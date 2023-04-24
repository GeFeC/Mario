#pragma once

#include "Controllers/BossController.hpp"
#include "LevelGenerator/LevelGenerator.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/BossState.hpp"
#include "res/textureGroups.hpp"

#include <memory>

static auto run_frame_level26(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level26;
  level.type = LevelState::Type::Boss;
  level.number = { 2, 6 };
  level.extra_textures = {
    texture_groups::green_koopa,
    texture_groups::blue_cloud,
    &textures::bg
  };

  run_frame_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::bg;
    level.cloud_textures = &texture_groups::blue_cloud;

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level26_1.csv");
    level_generator::generate_level(level, "level26_2.csv");

    auto& boss = level.bosses.king_koopa;
    boss = make_boss<KingKoopaState>(level);
    boss->position = glm::vec2(16, 6) * BlockBase::Size;

    level.stats.boss_hp = &boss->hp;
    level.stats.max_boss_hp = KingKoopaState::MaxHp;

    level.player.growth = PlayerState::Growth::Big;
    level.player.form = PlayerState::Form::Fire;
    level.player.position.x = 2 * BlockBase::Size;

    auto& bars = level.fire_bars;
    bars.push_back(FireBarState({ 4, 4 }, 4));
    bars.push_back(FireBarState({ 15, 4 }, 4));
  });
}
