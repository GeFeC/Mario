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
    &textures::green_mushroom,
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
    texture_allocators::yellow_goomba,
    texture_allocators::fire_flower,
    texture_allocators::green_koopa,
    texture_allocators::red_koopa,
    texture_allocators::beetle
  };

  const auto setup = [](auto& app){
    fonts::medium.allocate(32);
    fonts::small.allocate(16);

    auto& level = app.current_level;

    auto& player = level.player;

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
    player.position = { 60.f, 480.f };
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

    level.put_qblock_with_mushroom({ 11, 4 }, EntityState::DirectionRight);
    level.put_qblock_with_coins({ 22, 9 }, 1);
    
    for (int i = 0; i < 3; ++i){
      level.put_goomba({ 13 + i * 3, 10 });
    }
    level.put_red_koopa({ 9.f, 5.5f }, EntityState::DirectionRight);

    level.put_red_goomba({ 35, 5 });

    level.put_red_pipe({ 25, 10 }, 2);

    for (int i = 0; i < 10; ++i){
      level.put_bricks({ 55 + i, 8 });
      level.put_bricks({ 57 + i, 5 });
    }
    level.put_qblock_with_coins({ 67, 5 }, 1);
    
    level.put_red_pipe({ 71, 10 }, 2);
    level.put_qblock_with_coins({ 76, 7 }, 3);
    level.put_yellow_goomba({ 79, 10 });
    level.put_red_pipe({ 80, 10 }, 3);

    level.put_red_goomba({ 93, 6 });
    level.put_red_goomba({ 92, 3 });
    level.put_goomba({ 100, 10 });
    level.put_goomba({ 97, 10 });

    level.put_bricks({ 85, 7 });
    level.put_bricks({ 87, 7 });
    level.put_bricks({ 89, 7 });
    level.put_bricks({ 91, 7 });
    level.put_bricks({ 93, 7 });

    level.put_bricks({ 86, 4 });
    level.put_bricks({ 88, 4 });
    level.put_bricks({ 90, 4 });
    level.put_bricks({ 92, 4 });

    level.put_qblock_with_coins({ 86, 7 });
    level.put_qblock_with_coins({ 88, 7 });
    level.put_qblock_with_coins({ 90, 7 });
    level.put_qblock_with_coins({ 92, 7 });

    level.put_qblock_with_coins({ 87, 4 });
    level.put_qblock_with_mushroom({ 89, 4 });
    level.put_qblock_with_coins({ 91, 4 });

    level.put_goomba({ 64, 7 });
    level.put_goomba({ 66, 4 });
    level.put_red_goomba({ 68, 10 });

    for (int i = 0; i < 4; ++i){
      level.put_mushroom_bot({ 106, 11 - i });
    }
    level.put_mushroom_head({ 104, 7 }, 3);
    level.put_red_goomba({ 108, 6 });

    for (int i = 0; i < 3; ++i){
      level.put_coin({ 105 + i, 5 });
    }

    for (int i = 0; i < 5; ++i){
      level.put_mushroom_bot({ 116, 11 - i });
    }
    level.put_mushroom_head({ 113, 6 }, 5);
    level.put_yellow_goomba({ 119, 5 });

    for (int i = 0; i < 5; ++i){
      level.put_coin({ 114 + i, 4 });
    }

    for (int i = 0; i < 4; ++i){
      level.put_mushroom_bot({ 125, 11 - i });
    }
    level.put_mushroom_head({ 122, 7 }, 5);
    level.put_red_goomba({ 128, 6 }, EntityState::DirectionLeft, true);
    level.put_red_goomba({ 122, 6 }, EntityState::DirectionRight, true);

    for (int i = 0; i < 5; ++i){
      level.put_coin({ 123 + i, 5 });
    }

    for (int i = 0; i < 20; ++i){
      level.put_dirt({ 135.f + i, 11.f });
    }

    for (int i = 0; i < 3; ++i){
      level.put_mushroom_bot({ 140.f, 10 - i });
    }
    level.put_mushroom_head({ 138.f, 7 }, 3);
    level.put_yellow_goomba({ 138.f, 6 }, EntityState::DirectionRight);
    level.put_red_goomba({ 142.f, 6 }, EntityState::DirectionLeft, true);

    level.put_qblock_with_coins({ 139.f, 3 }, 5);
    level.put_qblock_with_green_mushroom({ 140.f, 3 });
    level.put_qblock_with_coins({ 141.f, 3 }, 5);

    level.put_red_koopa({ 146.f, 9.5f });

    for (int i = 0; i < 3; ++i){
      level.put_goomba({ 149.f + i * 2, 10 });
    }

    level.put_red_pipe({ 153.f, 10.f }, 3);
  };

  const auto loop = [](auto& app){
    level_controller(app.current_level);
    render_level(app.current_level);
  };

  run_frame(app, AppState::Frame::Level11, textures, setup, loop);
}
