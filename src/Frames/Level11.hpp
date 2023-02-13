#pragma once

//TEST LEVEL

#include "States/AppState.hpp"
#include "Views/Level.hpp"
#include "Controllers/LevelController.hpp"
#include "Frame.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

using Direction = EntityState::Direction;
static constexpr auto DirectionLeft = EntityState::DirectionLeft;

static auto put_dirt(LevelState& level, const glm::vec2& position){
  level.get_hitbox_grid_element(position) = 1;

  level.blocks.normal.push_back(BlockState(position, &textures::dirt));
};

auto put_bricks(LevelState& level, const glm::vec2& position){
  level.get_hitbox_grid_element(position) = 1;

  put_dirt(level, position);
  level.blocks.normal.back().is_visible = false;
  level.blocks.bricks.push_back(BricksBlockState(position));
  level.blocks.bricks.back().hitbox_block_index = level.blocks.normal.size() - 1;
};

auto put_mushroom_bot(LevelState& level, const glm::vec2& position){
  level.get_hitbox_grid_element(position) = 1;

  level.blocks.normal.push_back(BlockState(position, &textures::mushroom_bot));
  level.blocks.normal.back().is_solid = false;
};

auto put_mushroom_head(LevelState& level, const glm::vec2& position, int size){
  level.get_hitbox_grid_element(position) = 1;

  level.blocks.normal.push_back(BlockState(position, &textures::mushroom_left));

  for (int i = 0; i < size; ++i){
    level.get_hitbox_grid_element(position + glm::vec2(i + 1, 0)) = 1;
    level.blocks.normal.push_back(BlockState(position + glm::vec2(i + 1, 0), &textures::mushroom_center));
  }

  level.get_hitbox_grid_element(position + glm::vec2(size + 1, 0)) = 1;
  level.blocks.normal.push_back(BlockState(position + glm::vec2(size + 1, 0), &textures::mushroom_right));
}

auto put_coin(LevelState& level, const glm::vec2& position){
  level.blocks.coins.push_back(CoinBlockState(position));
}

auto put_spinning_coin(LevelState& level, const glm::vec2& position, int hits_required_to_bounce){
  level.blocks.spinning_coins.push_back(SpinningCoinState(position));

  level.blocks.spinning_coins.back().bounce_state.initial_power = -20.f;
  level.blocks.spinning_coins.back().bounce_state.hits_required_to_bounce = hits_required_to_bounce;
  
  if (hits_required_to_bounce > 0){
    level.blocks.spinning_coins.back().is_visible = false;
  }
}

auto put_qblock(LevelState& level, const glm::vec2& position, int bounces = 1){
  put_dirt(level, position);
  level.blocks.normal.back().is_visible = false;

  level.blocks.q_blocks.push_back(QBlockState(position));
  level.blocks.q_blocks.back().bounce_state.bounces_count = bounces;
}

auto put_qblock_with_coins(LevelState& level, const glm::vec2& position, int coins = 1){
  for (int i = 0; i < coins; ++i){
    put_spinning_coin(level, position, i + 1);
  }

  put_qblock(level, position, coins);

  auto& points = level.blocks.q_blocks.back().points_manager.points;
  points.reserve(coins);
  for (int i = 0; i < coins; ++i){
    auto particle = PointsParticlesState(QBlockState::RewardInPoints, position * config::BlockSize);
    particle.hits_required_to_activate = i + 1;

    points.push_back(particle);
  }
}

auto put_qblock_with_flower(LevelState& level, const glm::vec2& position){
  level.blocks.fire_flowers.push_back(FireFlowerState(position));
  level.blocks.fire_flowers.back().is_visible = false;

  put_qblock(level, position);
}

auto put_qblock_with_mushroom(LevelState& level, const glm::vec2& position, Direction direction = DirectionLeft){
  put_qblock(level, position, 1);
  level.entities.mushrooms.push_back(MushroomState::make_red(position, direction));
}

auto put_qblock_with_green_mushroom(LevelState& level, const glm::vec2& position, Direction direction = DirectionLeft){
  put_qblock(level, position, 1);
  level.entities.green_mushrooms.push_back(MushroomState::make_green(position, direction));
}

auto put_red_pipe(LevelState& level, const glm::vec2& position, int size){
  for (int i = 0; i < size; ++i){
    level.blocks.normal.push_back(BlockState({ position.x, position.y - i }, &textures::red_pipe_bottom_left));
    level.blocks.normal.push_back(BlockState({ position.x + 1, position.y - i }, &textures::red_pipe_bottom_right));
  }

  level.blocks.normal.push_back(BlockState({ position.x, position.y - size }, &textures::red_pipe_top_left));
  level.blocks.normal.push_back(BlockState({ position.x + 1, position.y - size }, &textures::red_pipe_top_right));
}

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
    texture_groups::beetle,
    texture_groups::spike,
    texture_groups::plant
  };

  const auto setup = [](auto& app){
    fonts::medium.allocate(32);
    fonts::small.allocate(16);

    auto& level = app.current_level;

    auto& background = level.background;
    auto& blocks = level.blocks;
    auto& entities = level.entities;

    auto& player = level.player;

    background.bushes = {
      BackgroundObjectState({ 3, 10 }, 2),
      BackgroundObjectState({ 13, 10 }, 3)
    };

    background.hills = {
      BackgroundObjectState({ 6, 10 }, 2),
      BackgroundObjectState({ 13, 10 }, 3)
    };

    player.death_delay = 0.5f;
    player.size = { 60.f, 60.f };
    player.position = { 60.f, 480.f };
    player.is_active = true;
    player.current_texture = &textures::small_mario;

    entities.green_flying_koopas.push_back(FlyingKoopaState::make_green({ 12, 9.5 }));

    for (int i = 0; i < 27; ++i){
      put_dirt(level, { i, 11 });
    }
    
    for (int i = 0; i < 50; ++i){
      put_dirt(level, { 50 + i, 11 });
    }

    for (int i = 0; i < 3; ++i){
      put_mushroom_bot(level, { 11, 10 - i });
    }
    put_mushroom_head(level, { 9, 7 }, 3);

    for (int i = 0; i < 5; ++i){
      put_mushroom_bot(level, { 33, 11 - i });
    }
    put_mushroom_head(level, { 31, 6 }, 3);
    
    for (int i = 0; i < 3; ++i){
      put_coin(level, { 32 + i, 5 });
    }

    for (int i = 0; i < 7; ++i){
      put_mushroom_bot(level, { 42, 11 - i });
    }
    put_mushroom_head(level, { 40, 4 }, 3);

    for (int i = 0; i < 3; ++i){
      put_coin(level, { 41 + i, 3 });
    }

    entities.red_goombas.push_back(GoombaState::make_red({ 42, 3 }));
    entities.red_koopas.push_back(KoopaState::make_red({ 9, 5.5 }));

    put_qblock_with_mushroom(level, { 11, 4 }, EntityState::DirectionRight);
    put_qblock_with_coins(level, { 22, 9 }, 1);
    
    for (int i = 0; i < 3; ++i){
      entities.goombas.push_back(GoombaState::make_normal({ 13 + i * 3, 10 }));
    }

    entities.red_goombas.push_back(GoombaState::make_red({ 35, 5 }));

    entities.plants.push_back(PlantState::make({ 25.5f, 8.f }));
    put_red_pipe(level, { 25, 10 }, 2);

    for (int i = 0; i < 10; ++i){
      put_bricks(level, { 55 + i, 8 });
      put_bricks(level, { 57 + i, 5 });
    }
    put_qblock_with_coins(level, { 67, 5 }, 1);
    
    put_red_pipe(level, { 71, 10 }, 2);
    put_qblock_with_coins(level, { 76, 7 }, 3);
    entities.yellow_goombas.push_back(GoombaState::make_yellow({ 79, 10 }));
    put_red_pipe(level, { 80, 10 }, 3);

    entities.red_goombas.push_back(GoombaState::make_red({ 93, 6 }));
    entities.red_goombas.push_back(GoombaState::make_red({ 92, 3 }));
    entities.goombas.push_back(GoombaState::make_normal({ 100, 10 }));
    entities.goombas.push_back(GoombaState::make_normal({ 97, 10 }));

    put_bricks(level, { 85, 7 });
    put_bricks(level, { 87, 7 });
    put_bricks(level, { 89, 7 });
    put_bricks(level, { 91, 7 });
    put_bricks(level, { 93, 7 });

    put_bricks(level, { 86, 4 });
    put_bricks(level, { 88, 4 });
    put_bricks(level, { 90, 4 });
    put_bricks(level, { 92, 4 });

    put_qblock_with_coins(level, { 86, 7 });
    put_qblock_with_coins(level, { 88, 7 });
    put_qblock_with_coins(level, { 90, 7 });
    put_qblock_with_coins(level, { 92, 7 });

    put_qblock_with_coins(level, { 87, 4 });
    put_qblock_with_mushroom(level, { 89, 4 });
    put_qblock_with_coins(level, { 91, 4 });

    entities.goombas.push_back(GoombaState::make_normal({ 64, 7 }));
    entities.goombas.push_back(GoombaState::make_normal({ 66, 4 }));
    entities.goombas.push_back(GoombaState::make_normal({ 68, 10 }));

    for (int i = 0; i < 4; ++i){
      put_mushroom_bot(level, { 106, 11 - i });
    }
    put_mushroom_head(level, { 104, 7 }, 3);
    entities.goombas.push_back(GoombaState::make_normal({ 108, 6 }));

    for (int i = 0; i < 3; ++i){
      put_coin(level, { 105 + i, 5 });
    }

    for (int i = 0; i < 5; ++i){
      put_mushroom_bot(level, { 116, 11 - i });
    }
    put_mushroom_head(level, { 113, 6 }, 5);
    entities.yellow_goombas.push_back(GoombaState::make_yellow({ 119, 5 }));

    for (int i = 0; i < 5; ++i){
      put_coin(level, { 114 + i, 4 });
    }

    for (int i = 0; i < 4; ++i){
      put_mushroom_bot(level, { 125, 11 - i });
    }
    put_mushroom_head(level, { 122, 7 }, 5);
    entities.red_goombas.push_back(GoombaState::make_red({ 128, 6 }, EntityState::DirectionLeft));
    entities.red_goombas.push_back(GoombaState::make_red({ 122, 6 }, EntityState::DirectionLeft));

    for (int i = 0; i < 5; ++i){
      put_coin(level, { 123 + i, 5 });
    }

    for (int i = 0; i < 20; ++i){
      put_dirt(level, { 135.f + i, 11.f });
    }

    for (int i = 0; i < 3; ++i){
      put_mushroom_bot(level, { 140.f, 10 - i });
    }
    put_mushroom_head(level, { 138.f, 7 }, 3);
    entities.yellow_goombas.push_back(GoombaState::make_yellow({ 138.f, 6 }, EntityState::DirectionRight));
    entities.red_goombas.push_back(GoombaState::make_red({ 142.f, 6 }, EntityState::DirectionRight));

    put_qblock_with_coins(level, { 139.f, 3 }, 5);
    put_qblock_with_green_mushroom(level, { 140.f, 3 });
    put_qblock_with_coins(level, { 141.f, 3 }, 5);

    entities.red_koopas.push_back(KoopaState::make_red({ 146.f, 9.5f }));

    for (int i = 0; i < 3; ++i){
      entities.goombas.push_back(GoombaState::make_normal({ 149.f + i * 2, 10 }));
    }

    put_red_pipe(level, { 153.f, 10.f }, 3);
  };

  const auto loop = [](auto& app){
    level_controller(app, app.current_level);
    render_level(app.current_level);
  };

  run_frame(app, AppState::Frame::Level11, textures, setup, loop);
}
