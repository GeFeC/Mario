#pragma once

#include "States/BlockState.hpp"
#include "States/EntityState.hpp"
#include "States/PointsParticlesState.hpp"

#include "Renderer/Text.hpp"
#include "config.hpp"
#include "res/fonts.hpp"
#include "res/textures.hpp"

#include <vector>

struct StatsState{
  int hp = 3;
  int score = 0;
  int time = 400;
  int coins = 0;
  int level_major = 1;
  int level_minor = 1;
};

struct LevelState{
  StatsState stats_state;

  EntityState player_state;

  std::vector<BlockState> blocks;
  std::vector<PointsParticlesState> points_particles;
  std::vector<EntityState> goombas;

  bool should_screen_scroll = false;
  float load_delay = 3.f;

  auto put_blocks(
      const glm::vec2& position, 
      Texture* texture, 
      const std::initializer_list<BlockState::Type>& types
  ){
    blocks.emplace_back(position * 60.f, texture, types);
  }

  auto put_dirt(const glm::vec2& position){
    put_blocks(position, &textures::dirt, { BlockState::Type::Solid });
  }

  auto put_bricks(const glm::vec2& position){
    put_blocks(position, &textures::dirt, { BlockState::Type::Solid });
    blocks.back().is_visible = false;
    put_blocks(position, &textures::bricks, { BlockState::Type::Solid, BlockState::Type::Bricks });
  }

  auto put_mushroom_bot(const glm::vec2& position){
    put_blocks(position, &textures::mushroom_bot, {});
  }

  auto put_mushroom_head(const glm::vec2& position, int size){
    put_blocks(position, &textures::mushroom_left, { BlockState::Type::Solid });

    for (int i = 0; i < size; ++i){
      put_blocks(position + glm::vec2(i + 1, 0.f), &textures::mushroom_center, { BlockState::Type::Solid });
    }

    put_blocks(position + glm::vec2(size + 1, 0.f), &textures::mushroom_right, { BlockState::Type::Solid });
  }
  
  auto put_coin(const glm::vec2& position){
    put_blocks(position, &textures::coin[0], { BlockState::Type::Coin });
  }

  auto put_spinning_coin(const glm::vec2& position, int hits_required_to_bounce){
    put_blocks(position, &textures::spinning_coin[0], { 
      BlockState::Type::SpinningCoin, 
    });

    blocks.back().bounce_state.initial_power = -20.f;
    blocks.back().bounce_state.hits_required_to_bounce = hits_required_to_bounce;
    
    if (hits_required_to_bounce > 0){
      blocks.back().is_visible = false;
    }
  }

  auto put_q_block(const glm::vec2& position, int coins = 1){
    for (int i = 0; i < coins; ++i){
      put_spinning_coin(position, i + 1);
    }

    put_blocks(position, &textures::dirt, { BlockState::Type::Solid });
    blocks.back().is_visible = false;

    put_blocks(position, &textures::q_block[0], { 
      BlockState::Type::Solid, 
      BlockState::Type::QBlock
    });

    blocks.back().bounce_state.bounces_count = coins;

    for (int i = 0; i < coins; ++i){
      auto particle = PointsParticlesState(
        config::RewardForQBlock, 
        position * 60.f, 
        PointsParticlesState::Type::QBlock
      );
      particle.hits_required_to_activate = i + 1;

      points_particles.push_back(particle);
    }
  }

  auto put_goomba(const glm::vec2& position){
    auto goomba = EntityState();

    goomba.position = position * 60.f;
    goomba.current_texture = &textures::goomba_walk[0];
    goomba.direction = EntityState::DirectionLeft;
    goomba.acceleration.left = config::GoombaWalkSpeed;
    goomba.points_index = points_particles.size();

    goombas.push_back(goomba);
    
    points_particles.emplace_back(config::RewardForKillingGoomba, glm::vec2(0), PointsParticlesState::Type::Entity);
  }
};
