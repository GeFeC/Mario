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
    &textures::mushroom_bg,
    &textures::mini_coin,
    &textures::mushroom,
    texture_allocators::bricks,
    texture_allocators::mario,
    texture_allocators::coin,
    texture_allocators::goomba,
    texture_allocators::mushroom,
    texture_allocators::q_block,
    texture_allocators::red_cloud,
    texture_allocators::spinning_coin,
    texture_allocators::red_bush,
    texture_allocators::red_hill,
    texture_allocators::red_pipe,
    texture_allocators::red_goomba,
    texture_allocators::fire_flower
  };

  const auto setup = [](auto& app){
    fonts::medium.allocate(32);
    fonts::small.allocate(16);

    auto& level = app.current_level_state;

    auto& player = level.player_state;

    level.background.bushes = {
      BackgroundObjectState({ 3, 10 }, 2),
      BackgroundObjectState({ 13, 10 }, 3)
    };

    level.background.hills = {
      BackgroundObjectState({ 6, 10 }, 2),
      BackgroundObjectState({ 13, 10 }, 3)
    };

    player.death_delay = 0.5f;
    player.size = { 60.f, 60.f };
    player.position = { 60.f, 540.f };
    player.is_active = true;
    player.current_texture = &textures::small_mario;

    for (int i = 0; i < 27; ++i){
      level.put_dirt({ i, 11 });
    }
    
    for (int i = 0; i < 50; ++i){
      level.put_dirt({ 50 + i, 11 });
    }

    for (int i = 0; i < 3; ++i){
      level.put_mushroom_bot({ 11, 10 - i });
    }
    level.put_mushroom_head({ 9, 7 }, 3);

    for (int i = 0; i < 5; ++i){
      level.put_mushroom_bot({ 33, 11 - i });
    }
    level.put_mushroom_head({ 31, 6 }, 3);
    
    for (int i = 0; i < 3; ++i){
      level.put_coin({ 32 + i, 5 });
    }

    for (int i = 0; i < 7; ++i){
      level.put_mushroom_bot({ 42, 11 - i });
    }
    level.put_mushroom_head({ 40, 4 }, 3);

    for (int i = 0; i < 3; ++i){
      level.put_coin({ 41 + i, 3 });
    }

    level.put_red_goomba({ 42, 3 });
    level.put_entity_hitbox_block({ 39, 3 });
    level.put_entity_hitbox_block({ 45, 3 });

    level.put_entity_hitbox_block({ 30, 5 });
    level.put_entity_hitbox_block({ 36, 5 });

    level.put_qblock_with_mushroom({ 11, 4 }, EntityState::DirectionRight);
    level.put_qblock_with_coins({ 22, 9 }, 1);
    
    for (int i = 0; i < 3; ++i){
      level.put_goomba({ 13 + i * 3, 10 });
    }

    level.put_red_goomba({ 35, 5 });

    level.put_red_pipe({ 25, 10 }, 2);

    for (int i = 0; i < 10; ++i){
      level.put_bricks({ 55 + i, 8 });
      level.put_bricks({ 57 + i, 5 });
    }

    level.put_qblock_with_flower({ 2, 8 });

    level.put_entity_hitbox_block({ 49, 10 });

    level.put_goomba({ 64, 7 });
    level.put_goomba({ 66, 4 });
    level.put_red_goomba({ 68, 10 });
  };

  const auto loop = [](auto& app){
    level_controller(app.current_level_state);
    render_level(app.current_level_state);
  };

  run_frame(app, AppState::Frame::Level11, textures, setup, loop);
}
