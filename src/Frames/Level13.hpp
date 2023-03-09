#pragma once

#include "States/AppState.hpp"
#include "States/HammerBroState.hpp"
#include "Views/Level.hpp"
#include "Controllers/LevelController.hpp"
#include "Frame.hpp"
#include "LevelGenerator.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"


static auto run_frame_level13(AppState& app){
  const auto textures = std::vector<TextureGroup>{
    &textures::black,
    &textures::dirt,
    &textures::mushroom_bg,
    &textures::mini_coin,
    &textures::mushroom,
    &textures::green_mushroom,
    texture_groups::bricks,
    texture_groups::mario,
    texture_groups::coin,
    texture_groups::goomba,
    texture_groups::mushroom,
    texture_groups::q_block,
    texture_groups::red_cloud,
    texture_groups::spinning_coin,
    texture_groups::red_bush,
    texture_groups::red_hill,
    texture_groups::red_pipe,
    texture_groups::red_goomba,
    texture_groups::yellow_goomba,
    texture_groups::fire_flower,
    texture_groups::green_koopa,
    texture_groups::green_flying_koopa,
    texture_groups::red_koopa,
    texture_groups::red_flying_koopa,
    texture_groups::beetle,
    texture_groups::spike,
    texture_groups::plant,
    texture_groups::red_plant,
    texture_groups::hammerbro
  };

  const auto setup = [](auto& app){
    fonts::normal.allocate(16);
    LevelState::timer = 0.f;

    auto& level = app.current_level;
    level.type = LevelState::Type::Vertical;
    level.generate_hitbox_grid();

    auto& player = level.player;
    player.position = { config::BlockSize, 195.f * config::BlockSize };

    level.stats.time = 600;
    level.stats.level_major = 1;
    level.stats.level_minor = 3;

    level.fire_bars.push_back(FireBarState({ 13, 183 }, 3));
    level.fire_bars.push_back(FireBarState({ 5, 154 }, 5));
    level.fire_bars.push_back(FireBarState({ 6, 111 }, 3));
    level.fire_bars.push_back(FireBarState({ 11, 108 }, 3));
    level.fire_bars.push_back(FireBarState({ 16, 105 }, 3));
    level.fire_bars.push_back(FireBarState({ 7, 95 }, 7));
    level.fire_bars.push_back(FireBarState({ 7, 74 }, 5));
    level.fire_bars.push_back(FireBarState({ 16, 39 }, 5));
    level.fire_bars.push_back(FireBarState({ 4, 4 }, 7));

    level_generator::put_qblock_with_coins(level, { 7, 25 }, 5);
    level_generator::put_qblock_with_coins(level, { 4, 115 }, 5);

    level_generator::generate_vertical_level_clouds(level);
    level_generator::generate_level(level, "../level13_1.csv");
    level_generator::generate_level(level, "../level13_2.csv");
  };

  const auto loop = [](auto& app){
    level_controller(app, app.current_level);
    render_level(app.current_level);
  };

  run_frame(app, AppState::Frame::Level13, textures, setup, loop);
}
