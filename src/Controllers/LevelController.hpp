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
    normal_goomba_controller(goomba, level);
  }

  for (auto& goomba : level.entities.red_goombas){
    red_goomba_controller(goomba, level);
  }

  for (auto& koopa : level.entities.green_koopas){
    koopa_controller(koopa, level, textures::green_koopa_walk);
  }

  for (auto& koopa : level.entities.red_koopas){
    koopa_controller(koopa, level, textures::red_koopa_walk);
  }
}

static auto entity_bounce_die(MonsterState& entity, LevelState& level, int reward){
  entity.gravity = -20.f;
  entity.should_collide = false;
  entity.vertical_flip = Drawable::Flip::UseFlip;

  level.stats_state.score += reward;
  entity.points_manager.get_points_particles().set_active(reward, entity.position);
}

static auto player_entity_interactions(PlayerState& player, LevelState& level){
  const auto kill_player_on_touch_ex = [&](EntityState& entity, const util::Rect& hitbox){
    auto entity_hitbox = EntityState();
    entity_hitbox.position = hitbox.position;
    entity_hitbox.size = hitbox.size;

    if (!player_stomp_on_entity(player, entity) && collision::is_hovering(player, entity_hitbox) && entity.can_kill()){
      if (player.growth == PlayerState::Growth::Big){
        player.is_shrinking = true;
      }
      else if (!player.is_shrinking && player.invincibility_delay <= 0.f) { 
        player.is_dead = true; 
      }
    }
  };

  const auto kill_player_on_touch = [&](MonsterState& entity){
    kill_player_on_touch_ex(entity, util::Rect(entity));
  };

  const auto die_when_stomped_ex 
        = [&](MonsterState& entity, const util::Rect& hitbox, int reward, auto set_entity_dead){
    auto entity_hitbox = EntityState();
    entity_hitbox.position = hitbox.position;
    entity_hitbox.size = hitbox.size;
    entity_hitbox.is_dead = entity.is_dead;

    if (player_stomp_on_entity(player, entity_hitbox) && !player.is_dead && !entity.is_dead && entity.should_collide){
      set_entity_dead();
      player.gravity = -15.f;

      level.stats_state.score += reward;
      entity.points_manager.make_next_points_particles();
      entity.points_manager.get_points_particles().set_active(reward * player.mobs_killed_in_row, entity.position);

      ++player.mobs_killed_in_row;

      return true;
    }

    return false;
  };

  const auto die_when_stomped = [&](MonsterState& entity, int reward, auto set_entity_dead){
    die_when_stomped_ex(entity, util::Rect(entity), reward, set_entity_dead);
  };

  const auto become_active_when_seen = [&](EntityState& entity){
    const auto screen_scroll = config::PlayerPositionToScroll - player.position.x;
    const auto view_extension = std::max(screen_scroll, 0.f);

    if (entity.position.x - player.position.x <= config::BlocksInRow * config::BlockSize + view_extension){
      entity.is_active = true;
    }
  };

  const auto die_when_hit_by_fireball = [&](MonsterState& entity, int reward){
    for (auto& fireball : player.fireballs){
      if (collision::is_hovering(fireball, entity) && fireball.is_active && entity.is_active){
        entity_bounce_die(entity, level, config::RewardForKillingGoomba);

        fireball.acceleration.left = fireball.acceleration.right = 0.f;
      }
    } 
  };

  auto& entities = level.entities;

  for (auto& goomba : level.entities.goombas){
    kill_player_on_touch(goomba);
    become_active_when_seen(goomba);
    die_when_hit_by_fireball(goomba, config::RewardForKillingGoomba);
    die_when_stomped(goomba, config::RewardForKillingGoomba, [&]{ goomba::normal_set_dead(goomba); });
  }

  for (auto& goomba : level.entities.red_goombas){
    kill_player_on_touch(goomba);
    become_active_when_seen(goomba);
    die_when_stomped(goomba, config::RewardForKillingGoomba, [&]{ goomba::red_set_dead(goomba); });
    die_when_hit_by_fireball(goomba, config::RewardForKillingGoomba);
  }

  const auto push_shell_on_player_touch = [&](auto& entity, int reward){
    if (!collision::is_hovering(player, entity) || entity.is_dead || !entity.should_collide) return;

    entity.shell_push_cooldown = glfwGetTime();
    entity.current_walk_speed = config::KoopaShellWalkSpeed;

    if (entity.position.y > player.position.y + 10.f){
      entity.points_manager.make_next_points_particles();
      auto& points = entity.points_manager.get_points_particles();

      const auto total_reward = reward * player.mobs_killed_in_row;
      points.set_active(total_reward, entity.position);
      level.stats_state.score += total_reward;
    }

    if (entity.position.x - player.position.x > 0){
      entity.set_direction(EntityState::DirectionRight, config::KoopaShellWalkSpeed);
      return;
    }

    entity.set_direction(EntityState::DirectionLeft, config::KoopaShellWalkSpeed);
  };

  const auto shell_entity_interactions = [&](
      auto& entity, 
      int reward, 
      int shell_speed,
      const Texture& dead_texture){
    if (entity.in_shell){
      entity.fall_from_edge = true;
    }

    if (entity.in_shell && entity.current_walk_speed == 0.f){
      push_shell_on_player_touch(entity, reward);
    }
    else {
      const auto entity_hitbox = entity.in_shell
      ? util::Rect(entity.position, glm::vec2(config::BlockSize))
      : util::Rect(
          entity.position + glm::vec2(0, config::BlockSize / 2), 
          glm::vec2(config::BlockSize)
        );

      const auto total_reward = reward * player.mobs_killed_in_row;

      if (glfwGetTime() - entity.shell_push_cooldown >= 0.2f){
        die_when_stomped_ex(entity, entity_hitbox, total_reward, [&]{ 
          koopa_hide_in_shell(entity, &dead_texture); 
        });
      }

      [&]{
        const auto distance = entity.position.x - player.position.x;
        if (distance > 0 && entity.acceleration.right == shell_speed) return;
        if (distance < 0 && entity.acceleration.left == shell_speed) return;

        if (entity.current_walk_speed > 0){
          kill_player_on_touch_ex(entity, entity_hitbox);
        }
      }();
    }
    become_active_when_seen(entity);
    die_when_hit_by_fireball(entity, reward);
  };

  for (auto& koopa : level.entities.green_koopas){
    shell_entity_interactions(
      koopa, 
      config::RewardForKillingKoopa, 
      config::KoopaShellWalkSpeed, 
      textures::green_koopa_dead
    );
  }

  for (auto& koopa : level.entities.red_koopas){
    shell_entity_interactions(
      koopa, 
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
