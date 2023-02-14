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
#include "Controllers/FlyingKoopaController.hpp"
#include "Controllers/BeetleController.hpp"
#include "Controllers/KoopaController.hpp"

#include "Util.hpp"
#include "config.hpp"
#include "res/textures.hpp"

static auto level_blocks_controller(LevelState& level){
  auto& player = level.player;
  auto& blocks = level.blocks;

  for (auto& block : blocks.q_blocks){
    q_block_controller(block, player, level.stats);
  }

  for (auto& block : blocks.bricks){
    bricks_controller(block, player, level);
  }

  for (auto& block : blocks.spinning_coins){
    spinning_coin_controller(block, player, level.stats);
  }

  for (auto& block : blocks.coins){
    coin_controller(block, player, level.stats);
  }
}

static auto level_entities_controller(LevelState& level){
  auto& entities = level.entities;
  auto& player = level.player;

  for (auto& goomba : level.entities.goombas){
    normal_goomba_controller(goomba, player, level);
  }

  for (auto& goomba : level.entities.red_goombas){
    red_goomba_controller(goomba, player, level);
  }

  for (auto& goomba : level.entities.yellow_goombas){
    yellow_goomba_controller(goomba, player, level);
  }

  for (auto& spike : level.entities.spikes){
    spike_controller(spike, player, level);
  }

  for (auto& plant : level.entities.plants){
    plant_controller(plant, player, level.stats);
  }

  for (auto& koopa : level.entities.green_koopas){
    green_koopa_controller(koopa, player, level);
  }

  for (auto& koopa : level.entities.green_flying_koopas){
    green_flying_koopa_controller(koopa, player, level);
  }

  for (auto& koopa : level.entities.red_koopas){
    red_koopa_controller(koopa, player, level);
  }

  for (auto& koopa : level.entities.red_flying_koopas){
    red_flying_koopa_controller(koopa, player, level);
  }

  for (auto& koopa : level.entities.beetles){
    beetle_controller(koopa, player, level);
  }
}

static auto level_block_entity_interactions(LevelState& level){
  const auto interact_with_block = [&](auto& entity, const auto& block){
    
  };

  auto& entities = level.entities;
  auto& goombas = entities.goombas;
  auto& red_goombas = entities.red_goombas;
  auto& yellow_goombas = entities.yellow_goombas;
  auto& red_koopas = entities.red_koopas;
  auto& green_koopas = entities.green_koopas;
  auto& green_flying_koopas = entities.green_flying_koopas;
  auto& mushrooms = entities.mushrooms;
  auto& beetles = entities.beetles;
  auto& spikes = entities.spikes;

  util::multi_for([&](const auto& block){
    util::multi_for([&](auto& entity){
      interact_with_block(entity, block);
    }, 
      goombas, 
      red_goombas, 
      red_koopas, 
      green_koopas, 
      green_flying_koopas, 
      yellow_goombas, 
      beetles,
      spikes
    );
  }, 
    level.blocks.bricks, 
    level.blocks.q_blocks
  ); 
}

static auto level_mushrooms_controller(LevelState& level){
  auto& player = level.player;

  for (auto& mushroom : level.entities.mushrooms){
    red_mushroom_controller(mushroom, player, level);
  }

  for (auto& mushroom : level.entities.green_mushrooms){
    green_mushroom_controller(mushroom, player, level);
  }
}

static auto level_screen_scroll(LevelState& level){
  level.should_screen_scroll = false;
  if (level.player.position.x > config::PlayerPositionToScroll){
    level.should_screen_scroll = true;
  }
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
  }

  //Blinking and counters
  LevelState::blink_state = blink_controller();
  LevelState::coin_spin_counter.run();
  LevelState::fire_flower_blink_counter.run();

  level.fireball_counter.run();

  auto& player = level.player;

  stats_controller(level.stats);
  player_controller(player, level);
  level_mushrooms_controller(level);

  for (auto& block : level.blocks.fire_flowers){
    fire_flower_controller(block, player, level.stats);
  }

  if (player.is_growing_up || player.is_shrinking || player.is_changing_to_fire) return;

  level_screen_scroll(level);

  level_blocks_controller(level);
  level_entities_controller(level);

  level_block_entity_interactions(level);
}
