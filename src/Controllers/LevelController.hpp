#pragma once

#include "Controllers/CollisionController.hpp"
#include "States/LevelState.hpp"
#include "States/LoopedCounter.hpp"

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
#include "Controllers/KoopaController.hpp"

#include "Util.hpp"
#include "config.hpp"
#include "res/textures.hpp"

static auto level_blocks_controller(LevelState& level, int blink_state){
  auto& player = level.player_state;
  auto& blocks = level.blocks;

  for (auto& block : level.blocks.q_blocks){
    bounce_controller(block);
    q_block_controller(block, player, level.stats_state);

    if (block.bounce_state.can_bounce){
      block.texture = &textures::q_block[blink_state];
    } 
  }

  for (auto& block : level.blocks.bricks){
    bounce_controller(block);
    bricks_controller(block, player, level);
  }

  static auto spin_counter = InfiniteCounter(4.f, 20.f);
  spin_counter.run();

  for (auto& block : level.blocks.spinning_coins){
    block.texture = &textures::spinning_coin[spin_counter.as_int()];
    spinning_coin_controller(block, player, level.stats_state);
    bounce_controller(block);
  }

  for (auto& block : level.blocks.coins){
    block.texture = &textures::coin[blink_state];
    coin_controller(block, player, level.stats_state);
  }

}

static auto level_entities_controller(LevelState& level){
  auto& entities = level.entities;

  for (auto& goomba : level.entities.goombas){
    goomba_controller(goomba, level, textures::goomba_walk);
  }

  for (auto& goomba : level.entities.red_goombas){
    goomba_controller(goomba, level, textures::red_goomba_walk);
  }

  for (auto& koopa : level.entities.green_koopas){
    koopa_controller(koopa, level, textures::green_koopa_walk);
  }

  for (auto& koopa : level.entities.red_koopas){
    koopa_controller(koopa, level, textures::red_koopa_walk);
  }
}

static auto player_entity_interactions(PlayerState& player, LevelState& level){
  auto& entities = level.entities;

  for (auto& goomba : level.entities.goombas){
    entity_kill_player_on_touch(goomba, player);
    entity_become_active_when_seen(goomba, player);
    entity_die_when_hit_by_fireball(goomba, player, level, config::RewardForKillingGoomba);
    entity_die_when_stomped(goomba, player, level, config::RewardForKillingGoomba, [&]{ 
      goomba_set_dead(goomba, &textures::goomba_dead);
    });
  }

  for (auto& goomba : level.entities.red_goombas){
    entity_kill_player_on_touch(goomba, player);
    entity_become_active_when_seen(goomba, player);
    entity_die_when_hit_by_fireball(goomba, player, level, config::RewardForKillingGoomba);
    entity_die_when_stomped(goomba, player, level, config::RewardForKillingGoomba, [&]{ 
      goomba_set_dead(goomba, &textures::red_goomba_dead);
    });
  }

  for (auto& koopa : level.entities.green_koopas){
    entity_handle_shell(
      koopa,
      player,
      level,
      config::RewardForKillingKoopa, 
      config::KoopaShellWalkSpeed, 
      textures::green_koopa_dead
    );
  }

  for (auto& koopa : level.entities.red_koopas){
    entity_handle_shell(
      koopa, 
      player,
      level,
      config::RewardForKillingKoopa, 
      config::KoopaShellWalkSpeed, 
      textures::red_koopa_dead
    );
  }

  for (auto& mushroom : level.entities.mushrooms){
    if (collision::is_hovering(player, mushroom) && mushroom.is_active){
      auto& points = mushroom.points_manager.get_points_particles();
      points.set_active(config::RewardForEatingMushroom, mushroom.position);
      level.stats_state.score += config::RewardForEatingMushroom;
    
      mushroom.is_active = false;
      mushroom.is_visible = false;
      mushroom.position.y = config::BigValue;
      player.is_growing_up = true;
    }

    const auto block = BouncingBlockState(mushroom.position / config::BlockSize);
    if (player_hit_block_above(player, block)){
      mushroom.should_be_pushed_out = true;
      mushroom.is_visible = true;
    } 
  }

  for (auto& flower : level.blocks.fire_flowers){
    const auto is_player_big = player.growth == PlayerState::Growth::Big;
    if (collision::is_hovering(player, flower) && flower.is_visible && is_player_big){
      flower.points_manager.get_points_particles().set_active(
        config::RewardForEatingFireFlower,
        flower.position
      );
      level.stats_state.score += config::RewardForEatingFireFlower;
    
      flower.is_visible = false;
      flower.position.y = config::BigValue;
      player.is_changing_to_fire = true;
    }
  }
}

static auto block_entity_interactions(MonsterState& entity, LevelState& level){
  for (const auto& block : level.blocks.bricks){
    if (block.bounce_state.is_bouncing){
      if (entity.is_dead || !entity.should_collide) return;

      const auto collision_state = collision_controller(util::Rect(entity), util::Rect(block));
      
      if (collision_state.distance_below | util::in_range(-15.f, 0.f)){
        entity_bounce_die(entity, level, config::RewardForKillingGoomba);
      }
    }
  }
}

static auto level_controller(LevelState& level){
  if (level.load_delay > 0.f) {
    level.load_delay -= window::delta_time;
    return;
  }

  level.fireball_counter.run();

  auto& player = level.player_state;

  level.should_screen_scroll = false;
  if (player.position.x > config::PlayerPositionToScroll){
    level.should_screen_scroll = true;
  }

  stats_controller(level.stats_state);
  player_controller(player, level);

  for (auto& mushroom : level.entities.mushrooms){
    mushroom_controller(mushroom, level);
  }

  static auto flower_blink_counter = InfiniteCounter(4.f, 15.f);
  flower_blink_counter.run();
  
  for (auto& block : level.blocks.fire_flowers){
    block.texture = &textures::fire_flower[flower_blink_counter.as_int()];
    fire_flower_controller(block, player);
  }

  if (player.is_growing_up || player.is_shrinking || player.is_changing_to_fire) return;

  const auto blink_state = blink_controller();

  level_blocks_controller(level, blink_state);
  level_entities_controller(level);

  player_entity_interactions(player, level);

  auto& goombas = level.entities.goombas;
  auto& red_goombas = level.entities.red_goombas;
  auto& red_koopas = level.entities.red_koopas;
  auto& green_koopas = level.entities.green_koopas;
  auto& mushrooms = level.entities.mushrooms;

  util::multi_for([&](auto& entity){
    block_entity_interactions(entity, level);
  }, goombas, red_goombas, mushrooms, green_koopas, red_koopas);
}
