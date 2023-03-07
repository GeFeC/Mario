#pragma once

//TEST LEVEL

#include "States/AppState.hpp"
#include "States/HammerBroState.hpp"
#include "Views/Level.hpp"
#include "Controllers/LevelController.hpp"
#include "Frame.hpp"
#include "LevelGenerator.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"


static auto run_frame_level11(AppState& app){
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
    player.position = { 190.f * config::BlockSize, config::BlockSize };

    level.stats.time = 400;
    level.stats.level_major = 1;
    level.stats.level_minor = 1;
    level_generator::generate_level(level, "../level11_tiles.csv");
    level_generator::generate_level(level, "../level11_Bushes.csv");
    level_generator::generate_level(level, "../level11_Hills.csv");
    level_generator::put_qblock_with_coins(level, { 77, 7 }, 5);
    level_generator::put_qblock_with_coins(level, { 187, 3 }, 5);
    level_generator::put_qblock_with_coins(level, { 189, 3 }, 5);
  };

  const auto loop = [](auto& app){
    level_controller(app, app.current_level);
    render_level(app.current_level);
  };

  run_frame(app, AppState::Frame::Level11, textures, setup, loop);
}
