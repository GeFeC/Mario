#pragma once

#include "States/BlockState.hpp"
#include "States/EntityState.hpp"
#include "States/PointsParticlesState.hpp"

#include "Renderer/Text.hpp"
#include "config.hpp"
#include "res/fonts.hpp"
#include "res/textures.hpp"
#include "Util.hpp"

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
  PlayerState player_state;

  struct Blocks{
    std::vector<BlockState> entity_hitbox_blocks;
    std::vector<BlockState> normal;
    std::vector<CoinBlockState> coins;
    std::vector<QBlockState> q_blocks;
    std::vector<BricksBlockState> bricks;
    std::vector<SpinningCoinState> spinning_coins;
  } blocks;

  struct Entities{
    std::vector<EntityState> goombas;
    std::vector<EntityState> red_goombas;
    std::vector<MushroomState> mushrooms;
  } entities;

  struct Background{
    std::vector<BackgroundObjectState> hills;
    std::vector<BackgroundObjectState> bushes;
  } background;

  std::vector<PointsParticlesState> points_particles;

  float load_delay = 3.f;
  bool should_screen_scroll = false;

  auto put_entity_hitbox_block(const glm::vec2& position){
    blocks.entity_hitbox_blocks.push_back(BlockState(position, &textures::dirt));
    //entity_hitbox_blocks.back().is_visible = false;
  }

  auto put_dirt(const glm::vec2& position){
    blocks.normal.push_back(BlockState(position, &textures::dirt));
  }

  auto put_bricks(const glm::vec2& position){
    put_dirt(position);
    blocks.normal.back().is_visible = false;
    blocks.bricks.push_back(BricksBlockState(position));
    blocks.bricks.back().hitbox_block_index = blocks.normal.size() - 1;
  }

  auto put_mushroom_bot(const glm::vec2& position){
    blocks.normal.push_back(BlockState(position, &textures::mushroom_bot));
    blocks.normal.back().is_solid = false;
  }

  auto put_mushroom_head(const glm::vec2& position, int size){
    blocks.normal.push_back(BlockState(position, &textures::mushroom_left));

    for (int i = 0; i < size; ++i){
      blocks.normal.push_back(BlockState(position + glm::vec2(i + 1, 0), &textures::mushroom_center));
    }

    blocks.normal.push_back(BlockState(position + glm::vec2(size + 1, 0), &textures::mushroom_right));
  }
  
  auto put_coin(const glm::vec2& position){
    blocks.coins.push_back(CoinBlockState(position));
  }

  auto put_spinning_coin(const glm::vec2& position, int hits_required_to_bounce){
    blocks.spinning_coins.push_back(SpinningCoinState(position));

    blocks.spinning_coins.back().bounce_state.initial_power = -20.f;
    blocks.spinning_coins.back().bounce_state.hits_required_to_bounce = hits_required_to_bounce;
    
    if (hits_required_to_bounce > 0){
      blocks.normal.back().is_visible = false;
    }
  }

  auto put_qblock(const glm::vec2& position, int bounces = 1){
    put_dirt(position);
    blocks.normal.back().is_visible = false;

    blocks.q_blocks.push_back(QBlockState(position));
    blocks.q_blocks.back().bounce_state.bounces_count = bounces;
  }

  auto put_qblock_with_coins(const glm::vec2& position, int coins = 1){
    for (int i = 0; i < coins; ++i){
      put_spinning_coin(position, i + 1);
    }

    put_qblock(position, coins);

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

  using Direction = EntityState::Direction;

  auto put_goomba(const glm::vec2& position, EntityState::Direction direction = EntityState::DirectionLeft){
    auto goomba = EntityState();

    goomba.position = position * 60.f;
    goomba.current_texture = &textures::goomba_walk[0];
    goomba.set_direction(direction, config::GoombaWalkSpeed);
    goomba.points_index = points_particles.size();

    entities.goombas.push_back(goomba);
    
    points_particles.emplace_back(config::RewardForKillingGoomba, glm::vec2(0), PointsParticlesState::Type::Entity);
  }

  auto put_red_goomba(const glm::vec2& position, EntityState::Direction direction = EntityState::DirectionLeft){
    auto goomba = EntityState();

    goomba.position = position * 60.f;
    goomba.current_texture = &textures::red_goomba_walk[0];
    goomba.set_direction(direction, config::GoombaWalkSpeed);
    goomba.points_index = points_particles.size();

    entities.red_goombas.push_back(goomba);
    
    points_particles.emplace_back(config::RewardForKillingGoomba, glm::vec2(0), PointsParticlesState::Type::Entity);
  }
  
  auto put_mushroom(const glm::vec2& position, EntityState::Direction direction = EntityState::DirectionLeft){
    auto mushroom = MushroomState();

    mushroom.position = position * 60.f;
    mushroom.current_texture = &textures::mushroom;
    mushroom.set_direction(direction, config::MushroomWalkSpeed);

    mushroom.points_index = points_particles.size();
    mushroom.is_visible = false;

    entities.mushrooms.push_back(mushroom);
    
    points_particles.emplace_back(config::RewardForEatingMushroom, glm::vec2(0), PointsParticlesState::Type::Entity);
  }

  auto put_qblock_with_mushroom(const glm::vec2& position, Direction direction = EntityState::DirectionLeft){
    put_qblock(position, 1);
    put_mushroom(position, direction);
  }

  auto put_red_pipe(const glm::vec2& position, int size){
    for (int i = 0; i < size; ++i){
      blocks.normal.push_back(BlockState({ position.x, position.y - i }, &textures::red_pipe_bottom_left));
      blocks.normal.push_back(BlockState({ position.x + 1, position.y - i }, &textures::red_pipe_bottom_right));
    }

    blocks.normal.push_back(BlockState({ position.x, position.y - size }, &textures::red_pipe_top_left));
    blocks.normal.push_back(BlockState({ position.x + 1, position.y - size }, &textures::red_pipe_top_right));
  }
};
