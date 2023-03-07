#pragma once

#include "States/LevelState.hpp"
#include "States/LoopedCounter.hpp"
#include "States/AppState.hpp"

#include "Controllers/CollisionController.hpp"
#include "Controllers/PlayerController.hpp"
#include "Controllers/BlinkController.hpp"
#include "Controllers/CoinController.hpp"
#include "Controllers/BounceController.hpp"
#include "Controllers/QBlockController.hpp"
#include "Controllers/SpinningCoinController.hpp"
#include "Controllers/StatsController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/GoombaController.hpp"
#include "Controllers/MushroomController.hpp"
#include "Controllers/BricksController.hpp"
#include "Controllers/FireFlowerController.hpp"
#include "Controllers/ShellMonsterController.hpp"
#include "Controllers/SpikeController.hpp"
#include "Controllers/PlantController.hpp"
#include "Controllers/JumpingKoopaController.hpp"
#include "Controllers/FlyingKoopaController.hpp"
#include "Controllers/BeetleController.hpp"
#include "Controllers/KoopaController.hpp"
#include "Controllers/FireBarController.hpp"
#include "Controllers/HammerBroController.hpp"

#include "Util/Util.hpp"
#include "Window.hpp"
#include "config.hpp"
#include "res/textures.hpp"

static auto level_blocks_controller(LevelState& level){
  auto& player = level.player;
  auto& blocks = level.blocks;

  for (auto& block : blocks.q_blocks){
    q_block_controller(block, level);
  }

  for (auto& block : blocks.bricks){
    bricks_controller(block, level);
  }

  for (auto& block : blocks.spinning_coins){
    spinning_coin_controller(block, level);
  }

  for (auto& block : blocks.coins){
    coin_controller(block, level);
  }
}

static auto level_entities_controller(LevelState& level){
  auto& entities = level.entities;
  auto& player = level.player;

  for (auto& goomba : level.entities.goombas){
    normal_goomba_controller(goomba, level);
  }

  for (auto& goomba : level.entities.red_goombas){
    red_goomba_controller(goomba, level);
  }

  for (auto& goomba : level.entities.yellow_goombas){
    yellow_goomba_controller(goomba, level);
  }

  for (auto& spike : level.entities.spikes){
    spike_controller(spike, level);
  }

  for (auto& plant : level.entities.plants){
    green_plant_controller(plant, level);
  }

  for (auto& plant : level.entities.red_plants){
    red_plant_controller(plant, level);
  }

  for (auto& koopa : level.entities.green_koopas){
    green_koopa_controller(koopa, level);
  }

  for (auto& koopa : level.entities.green_jumping_koopas){
    green_jumping_koopa_controller(koopa, level);
  }

  for (auto& koopa : level.entities.green_flying_koopas){
    green_flying_koopa_controller(koopa, level);
  }

  for (auto& koopa : level.entities.red_koopas){
    red_koopa_controller(koopa, level);
  }

  for (auto& koopa : level.entities.red_jumping_koopas){
    red_jumping_koopa_controller(koopa, level);
  }

  for (auto& koopa : level.entities.red_flying_koopas){
    red_flying_koopa_controller(koopa, level);
  }

  for (auto& koopa : level.entities.beetles){
    beetle_controller(koopa, level);
  }

  for (auto& bro : level.entities.hammerbros){
    hammerbro_controller(bro, level);
  }
}

static auto level_mushrooms_controller(LevelState& level){
  auto& player = level.player;

  for (auto& mushroom : level.entities.mushrooms){
    red_mushroom_controller(mushroom, level);
  }

  for (auto& mushroom : level.entities.green_mushrooms){
    green_mushroom_controller(mushroom, level);
  }
}

static auto level_screen_scroll(LevelState& level){
  level.should_screen_scroll = false;
  if (level.player.position.x > config::PlayerPositionToScroll){
    level.should_screen_scroll = true;
  }
}

static auto level_finish(LevelState& level, AppState& app){
  const auto [finish_x, finish_y] = LevelState::FinishingPipePosition; 
  auto& player = level.player;

  if (level.is_finished){
    level.score_adding_after_finish_delay -= window::delta_time;
    level.player.position.y += window::delta_time * config::BlockSize;

    if (level.stats.time > 0) {
      if (level.score_adding_after_finish_delay <= 0.f){
        level.score_adding_after_finish_delay = 1.f / 60.f;
        level.stats.time--;
        level.stats.score += 50;
      }
    }
    else{
      level.finish_delay -= window::delta_time;

      if (level.finish_delay <= 0.f){
        app.should_restart_current_frame = true;
        app.current_frame = AppState::Frame::Level12;
      }
    }
  }

  if (player.position.y / config::BlockSize < finish_y - 1) return;
  if (player.position.x / config::BlockSize != util::in_range(finish_x, finish_x + 1)) return;

  if (window::is_key_pressed(GLFW_KEY_DOWN)) level.is_finished = true;
}

static auto level_controller(AppState& app, LevelState& level){
  //Level loading
  if (level.load_delay > 0.f) {
    level.load_delay -= window::delta_time;
    return;
  }

  //Falling under the level
  if (level.player.position.y > config::PlayerPositionToRestartLevel){
    app.should_restart_current_frame = true;
    level.stats.hp--;

    if (level.stats.hp == 0){
      level.stats = StatsState{};
      app.current_frame = AppState::Frame::Level11;
    }
  }

  //Blinking and counters
  LevelState::blink_state = blink_controller();
  LevelState::coin_spin_counter.run();
  LevelState::fire_flower_blink_counter.run();

  level.fireball_counter.run();
  level.hammer_counter.run();

  auto& player = level.player;

  if (!level.is_finished) {
    stats_controller(level.stats);
    player_controller(player, level);
  }

  level_mushrooms_controller(level);

  for (auto& block : level.blocks.fire_flowers){
    fire_flower_controller(block, player, level.stats);
  }

  for (auto& bar : level.fire_bars){
    fire_bar_controller(bar, level);
  }

  if (player.is_growing_up || player.is_shrinking || player.is_changing_to_fire) return;

  level_screen_scroll(level);
  level_finish(level, app);

  level_blocks_controller(level);
  level_entities_controller(level);

  LevelState::timer += window::delta_time;
}
