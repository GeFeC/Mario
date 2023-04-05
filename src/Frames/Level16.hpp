#pragma once

#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/BossState.hpp"
#include "res/textureGroups.hpp"

static auto run_frame_level16(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level16;
  level.type = LevelState::Type::Boss;
  level.number = { 1, 6 };
  level.extra_textures = {
    texture_groups::red_cloud,
    texture_groups::q_block,
    texture_groups::king_goomba,
    texture_groups::goomba,
    &textures::mushroom_bg
  };

  run_frame_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::mushroom_bg;

    auto& boss = level.bosses.king_goomba; 
    boss = std::make_unique<KingGoombaState>();
    boss->position = glm::vec2(16, 8) * config::BlockSize;
    boss->walk_speed = 7.f;
    boss->set_direction(EntityState::DirectionLeft);

    level.stats.boss_hp = &boss->hp;
    level.stats.max_boss_hp = KingGoombaState::MaxHp;

    level.player.growth = PlayerState::Growth::Big;
    level.player.form = PlayerState::Form::Fire;
    level.player.position.x = 7 * config::BlockSize;

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level16_1.csv");
    level_generator::generate_level(level, "level16_2.csv");
  });
}
