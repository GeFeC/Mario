#pragma once

#include "States/AppState.hpp"
#include "Views/Level.hpp"
#include "Controllers/LevelController.hpp"
#include "Frame.hpp"
#include "res/textureAllocators.hpp"
#include "res/textures.hpp"

static auto run_frame_level11(AppState& app){
  const auto textures = std::vector<TextureAllocator>{
    &textures::black,
    &textures::dirt,
    &textures::bricks,
    &textures::mushroom_bg,
    &textures::mini_coin,
    texture_allocators::mario,
    texture_allocators::coin,
    texture_allocators::goomba,
    texture_allocators::mushroom,
    texture_allocators::q_block,
    texture_allocators::red_cloud,
    texture_allocators::spinning_coin
  };

  const auto setup = [](auto& app){
    fonts::medium.allocate(32);
    fonts::small.allocate(16);

    auto& level = app.current_level_state;

    level.player_state.position = { 60.f, 600.f };
    level.player_state.is_active = true;

    for (int i = 0; i < 120; ++i){
      level.put_dirt({ i, 11 });
    }

    for (int i = 0; i < 3; ++i){
      level.put_mushroom_bot({ 11, 10 - i });
    }

    level.put_mushroom_head({ 9, 7 }, 3);

    level.put_q_block({ 11, 4 }, 1);
    
    for (int i = 0; i < 3; ++i){
      level.put_goomba({ 23 + i * 2, 10 });
    }
  };

  const auto loop = [](auto& app){
    level_controller(app.current_level_state);
    render_level(app.current_level_state);
  };

  run_frame(app, AppState::Frame::Level11, textures, setup, loop);
}
