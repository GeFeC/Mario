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
#include "Controllers/MushroomController.hpp"
#include "Controllers/BricksController.hpp"

#include "Util.hpp"
#include "config.hpp"
#include "res/textures.hpp"

static auto level_blocks_controller(LevelState& level, int blink_state){
  const auto animation_state = animation_controller();

  auto& player = level.player_state;
  auto& blocks = level.blocks;

  for (auto& block : level.blocks.q_blocks){
    bounce_controller(block, player);
    q_block_controller(block, player, level.stats_state);

    if (block.bounce_state.can_bounce){
      block.texture = &textures::q_block[blink_state];
    } 
  }

  for (auto& block : level.blocks.bricks){
    bounce_controller(block, player);
    bricks_controller(block, player, level);
  }

  for (auto& block : level.blocks.spinning_coins){
    block.texture = &textures::spinning_coin[animation_state];
    spinning_coin_controller(block, player, level.stats_state);
    bounce_controller(block, player);
  }

  for (auto& block : level.blocks.coins){
    block.texture = &textures::coin[blink_state];
    coin_controller(block, player, level.stats_state);
  }
}

static auto level_entities_controller(LevelState& level){
  for (auto& goomba : level.entities.goombas){
    entity_gravity(goomba, level);
    entity_movement(goomba, level);
    entity_turn_around(goomba, config::GoombaWalkSpeed);
    normal_goomba_controller(goomba);
  }

  for (auto& goomba : level.entities.red_goombas){
    entity_gravity(goomba, level);
    entity_movement_with_turning(goomba, level);
    entity_turn_around(goomba, config::GoombaWalkSpeed);
    red_goomba_controller(goomba);
  }

  for (auto& mushroom : level.entities.mushrooms){
    entity_gravity(mushroom, level);
    entity_movement(mushroom, level);
    entity_turn_around(mushroom, config::MushroomWalkSpeed);

    mushroom_controller(mushroom, level.player_state);
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

static auto player_entity_interactions(PlayerState& player, LevelState& level){
  const auto kill_player_on_touch = [&](auto& goomba){
    if (collision::is_hovering(player, goomba) && !goomba.is_dead && goomba.should_collide){
      if (player.growth == PlayerState::Growth::Big){
        player.is_shrinking = true;
      }
      else if (!player.is_shrinking && player.invincibility_delay <= 0.f) { 
        player.is_dead = true; 
      }
    }
  };

  const auto die_when_stomped = [&](auto& goomba, auto set_entity_dead){
    if (player_stomp_on_entity(player, goomba) && !player.is_dead){
      set_entity_dead();
      player.gravity = -15.f;

      receive_points_for_killing_entity(goomba, level, config::RewardForKillingGoomba * player.mobs_killed_in_row);
      ++player.mobs_killed_in_row;
    }
  };

  const auto become_active_when_seen = [&](auto& goomba){
    const auto screen_scroll = config::PlayerPositionToScroll - player.position.x;
    const auto view_extension = std::max(screen_scroll, 0.f);

    if (goomba.position.x - player.position.x <= config::BlocksInRow * config::BlockSize + view_extension){
      goomba.is_active = true;
    }
  };

  for (auto& goomba : level.entities.goombas){
    die_when_stomped(goomba, [&]{ goomba::normal_set_dead(goomba); });
    kill_player_on_touch(goomba);
    become_active_when_seen(goomba);
  }

  for (auto& goomba : level.entities.red_goombas){
    die_when_stomped(goomba, [&]{ goomba::red_set_dead(goomba); });
    kill_player_on_touch(goomba);
    become_active_when_seen(goomba);
  }

  for (auto& mushroom : level.entities.mushrooms){
    if (collision::is_hovering(player, mushroom) && mushroom.is_active){
      auto& points = level.points_particles[mushroom.points_index];
      points.set_active(mushroom.position);
      level.stats_state.score += config::RewardForEatingMushroom;
    
      mushroom.is_active = false;
      mushroom.is_visible = false;
      mushroom.position.y = config::BigValue;
      player.is_growing_up = true;
    }
  }
}

static auto block_entity_interactions(EntityState& entity, LevelState& level){
  for (const auto& block : level.blocks.bricks){
    if (block.bounce_state.is_bouncing){
      const auto collision_state = collision_controller(util::Rect(entity), util::Rect(block));
      
      if (collision_state.distance_below | util::in_range(-15.f, 0.f)){
        entity.gravity = -20.f;
        entity.should_collide = false;
        entity.vertical_flip = Drawable::Flip::UseFlip;
        receive_points_for_killing_entity(entity, level, config::RewardForKillingGoomba);
      }
    }
  }
}

template<typename T>
class TD;

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
  
  for (auto& particle : level.points_particles){
    if (particle.type == PointsParticlesState::Type::QBlock){
      q_block_points_controller(particle, player);
    }

    points_particles_controller(particle);
  } 

  if (player.is_growing_up || player.is_shrinking) return;

  entity_movement(player, level);

  const auto blink_state = blink_controller();

  level_blocks_controller(level, blink_state);
  level_entities_controller(level);

  player_entity_interactions(player, level);

  auto& [goombas, red_goombas, mushrooms] = level.entities;

  util::multi_for([&](auto& entity){
    block_entity_interactions(entity, level);
  }, goombas, red_goombas, mushrooms);
}
