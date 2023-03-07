#pragma once

#include "States/AppState.hpp"
#include "States/HammerBroState.hpp"
#include "Views/Level.hpp"
#include "Controllers/LevelController.hpp"
#include "Frame.hpp"
#include "LevelGenerator.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"


static auto run_frame_level12(AppState& app){
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

    auto& player = level.player;
    player.position = { config::BlockSize, 1 * config::BlockSize };

    level.stats.time = 400;
    level.stats.level_major = 1;
    level.stats.level_minor = 2;
    level_generator::generate_level(level, "../level12_1.csv");
    level_generator::generate_level(level, "../level12_2.csv");

    level_generator::put_qblock_with_coins(level, { 84, 2 }, 5);

    level_generator::put_qblock_with_coins(level, { 178, 3 }, 5);
    level_generator::put_qblock_with_coins(level, { 180, 3 }, 5);
    level_generator::put_qblock_with_coins(level, { 182, 3 }, 5);
  };

  const auto loop = [](auto& app){
    level_controller(app, app.current_level);
    render_level(app.current_level);
  };

  run_frame(app, AppState::Frame::Level12, textures, setup, loop);
}
