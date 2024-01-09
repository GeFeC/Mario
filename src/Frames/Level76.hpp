#pragma once

#include "LevelGenerator/LevelGenerator.hpp"
#include "States/BossState.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textures.hpp"

namespace mario::frames{

static auto run_level76(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level76;
  level.type = LevelState::Type::Boss;
  level.number = { 7, 6 };
  level.extra_textures = {
    &textures::castle_bg,
		&textures::peach,
		texture_groups::king_bowser,
		texture_groups::lava
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::castle_bg;

		level.game_objects.push(LavaState{ glm::vec2(3.f, 11.f), glm::vec2(14.f, 1.f) });

    level_generator::generate_level(level, "level76_1.csv");
    level_generator::generate_level(level, "level76_2.csv");

    auto& objects = level.game_objects;
    auto& boss = objects.push(KingBowserState());

    level.stats.boss_hp = { &boss.hp, KingBowserState::MaxHp };

    level.player.set_form_to_fire();
    level.player.position = glm::vec2(2.f, 9.f) * BlockBase::Size;
  });
}

} //namespace mario::frames
