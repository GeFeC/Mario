#pragma once

#include "Controllers/CollisionController.hpp"
#include "States/LevelState.hpp"

#include "Controllers/PlayerController.hpp"
#include "Controllers/BlinkController.hpp"
#include "Controllers/CoinController.hpp"
#include "Controllers/BounceController.hpp"
#include "Controllers/AnimationController.hpp"
#include "Controllers/QBlockController.hpp"
#include "Controllers/SpinningCoinController.hpp"
#include "Controllers/StatsController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/GoombaController.hpp"

#include "Util.hpp"
#include "config.hpp"
#include "res/textures.hpp"

static auto level_blocks_controller(LevelState& level, int blink_state){
  const auto animation_state = animation_controller();

  auto& player = level.player_state;

  for (auto& block : level.blocks){
    bounce_controller(block, player);

    if (block.types | util::contains(BlockState::Type::Coin)){
      block.texture = &textures::coin[blink_state];

      coin_controller(block, player, level.stats_state);
    }

    if (block.types | util::contains(BlockState::Type::QBlock)){
      block.texture = &textures::q_block[blink_state];
      q_block_controller(block, player, level.stats_state);
    }

    if (block.types | util::contains(BlockState::Type::SpinningCoin)){
      block.texture = &textures::spinning_coin[animation_state];
      spinning_coin_controller(block, player);
    }

    if (block.types | util::contains(BlockState::Type::Bricks)){
      if (player_hit_block_above(player, block)){
        bounce::start(block);
      }
    }
  }
}

static auto level_entities_controller(LevelState& level){
  for (auto& goomba : level.goombas){
    entity_gravity(goomba, level);
    entity_movement(goomba, level);
    entity_turn_around(goomba, config::GoombaWalkSpeed);
    goomba_controller(goomba);
  }
}

static auto receive_points_for_killing_entity(const EntityState& entity, LevelState& level, int reward){
  level.stats_state.score += reward;

  auto& points = level.points_particles[entity.points_index];
  points.text.set_text(std::to_string(reward));
  points.text.set_position(entity.position);
  points.is_active = true;
  points.text.is_visible = true;
}

static auto player_entity_interactions(EntityState& player, LevelState& level){
  for (auto& goomba : level.goombas){
    if (player_stomp_on_entity(player, goomba) && !player.is_dead){
      goomba::on_dead(goomba);
      player.gravity = -15.f;

      receive_points_for_killing_entity(goomba, level, config::RewardForKillingGoomba * player.mobs_killed_in_row);
      player.mobs_killed_in_row *= 2;
    }

    if (collision::is_hovering(player, goomba) && !goomba.is_dead){
      player.is_dead = true;
    }

    if (goomba.position.x - player.position.x <= config::BlocksInRow * 60){
      goomba.is_active = true;
    }
  }
}


static auto level_controller(LevelState& level){
  if (level.load_delay > 0.f) {
    level.load_delay -= window::delta_time;
    return;
  }

  auto& player = level.player_state;

  level.should_screen_scroll = false;
  if (player.position.x > config::PlayerPositionToScroll){
    level.should_screen_scroll = true;
  }

  stats_controller(level.stats_state);
  player_controller(player, level);
  entity_movement(player, level);

  const auto blink_state = blink_controller();

  level_blocks_controller(level, blink_state);
  level_entities_controller(level);

  player_entity_interactions(player, level);

  for (auto& particle : level.points_particles){
    if (particle.type == PointsParticlesState::Type::QBlock){
      q_block_points_controller(particle, player);
    }

    points_particles_controller(particle);
  } 
}
