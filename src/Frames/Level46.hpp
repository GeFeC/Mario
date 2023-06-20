#pragma once

#include "Controllers/KingCheepController.hpp"
#include "LevelGenerator/LevelGenerator.hpp"
#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/BossState.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

namespace mario::frames{

static auto run_level46(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level46;
  level.type = LevelState::Type::Boss;
  level.number = { 4, 6 };
  level.extra_textures = {
    texture_groups::red_fish,
    texture_groups::grey_fish,
    &textures::water_bottom,
    &textures::water_top,
    &textures::bg
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::bg;
    level.water_level = 0.f;

    level_generator::generate_level(level, "level46_1.csv");
    level_generator::generate_level(level, "level46_2.csv");

    auto& objects = level.game_objects;

    //Fish generator
    for (int i = 0; i < 2; ++i){
      //Fish appearing from right:

      objects.push(FishState::make_red({ 
        king_cheep_controller::minion_random_position_from_right()
      })).is_active = true;

      objects.push(FishState::make_grey({ 
        king_cheep_controller::minion_random_position_from_right()
      })).is_active = true;

      //Fish appearing from left:
      
      objects.push(FishState::make_red({ 
        king_cheep_controller::minion_random_position_from_left()
      })).set_direction(FishState::DirectionRight);

      objects.push(FishState::make_grey({ 
        king_cheep_controller::minion_random_position_from_left()
      })).set_direction(FishState::DirectionRight);
    }

    auto& boss = objects.push(KingCheepState());

    boss.position = glm::vec2(16, 6) * BlockBase::Size;

    level.stats.boss_hp = &boss.hp;
    level.stats.max_boss_hp = KingCheepState::MaxHp;

    level.player.growth = PlayerState::Growth::Big;
    level.player.form = PlayerState::Form::Fire;
    level.player.position.x = 2 * BlockBase::Size;
  });
}

} //namespace mario::frames
