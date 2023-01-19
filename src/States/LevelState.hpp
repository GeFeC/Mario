#pragma once

#include "States/BlockState.hpp"
#include "States/EntityState.hpp"
#include "States/MonsterState.hpp"
#include "States/PlayerState.hpp"
#include "States/PointsParticlesState.hpp"
#include "States/LoopedCounter.hpp"
#include "States/QBlockState.hpp"
#include "States/FireFlowerState.hpp"

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
  util::vector2d<int> hitbox_grid;

  StatsState stats;
  PlayerState player;

  struct Blocks{
    std::vector<BlockState> entity_hitbox_blocks;
    std::vector<BlockState> normal;
    std::vector<CoinBlockState> coins;
    std::vector<QBlockState> q_blocks;
    std::vector<BricksBlockState> bricks;
    std::vector<SpinningCoinState> spinning_coins;
    std::vector<FireFlowerState> fire_flowers;
  } blocks;

  struct Entities{
    std::vector<GoombaState> goombas;
    std::vector<GoombaState> red_goombas;
    std::vector<GoombaState> yellow_goombas;
    std::vector<KoopaState> green_koopas;
    std::vector<KoopaState> red_koopas;
    std::vector<BeetleState> beetles;
    std::vector<MushroomState> mushrooms;
  } entities;

  struct Background{
    std::vector<BackgroundObjectState> hills;
    std::vector<BackgroundObjectState> bushes;
  } background;

  InfiniteCounter fireball_counter;

  float load_delay = 3.f;
  bool should_screen_scroll = false;

  LevelState() : fireball_counter(4.f, 20.f) {
    hitbox_grid.resize(config::MaxLevelSize, std::vector<int>(config::BlocksInColumn, 0));
  }

  auto& get_hitbox_grid_element(const glm::vec2& position){
    return hitbox_grid[position.x][position.y];
  }

  auto put_dirt(const glm::vec2& position){
    get_hitbox_grid_element(position) = 1;

    blocks.normal.push_back(BlockState(position, &textures::dirt));
  }

  auto put_bricks(const glm::vec2& position){
    get_hitbox_grid_element(position) = 1;

    put_dirt(position);
    blocks.normal.back().is_visible = false;
    blocks.bricks.push_back(BricksBlockState(position));
    blocks.bricks.back().hitbox_block_index = blocks.normal.size() - 1;
  }

  auto put_mushroom_bot(const glm::vec2& position){
    get_hitbox_grid_element(position) = 1;

    blocks.normal.push_back(BlockState(position, &textures::mushroom_bot));
    blocks.normal.back().is_solid = false;
  }

  auto put_mushroom_head(const glm::vec2& position, int size){
    get_hitbox_grid_element(position) = 1;

    blocks.normal.push_back(BlockState(position, &textures::mushroom_left));

    for (int i = 0; i < size; ++i){
      get_hitbox_grid_element(position + glm::vec2(i + 1, 0)) = 1;
      blocks.normal.push_back(BlockState(position + glm::vec2(i + 1, 0), &textures::mushroom_center));
    }

    get_hitbox_grid_element(position + glm::vec2(size + 1, 0)) = 1;
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
      blocks.spinning_coins.back().is_visible = false;
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

    auto& points = blocks.q_blocks.back().points_manager.points;
    points.reserve(coins);
    for (int i = 0; i < coins; ++i){
      auto particle = PointsParticlesState(config::RewardForQBlock, position * config::BlockSize);
      particle.hits_required_to_activate = i + 1;

      points.push_back(particle);
    }
  }

  auto put_qblock_with_flower(const glm::vec2& position){
    blocks.fire_flowers.push_back(FireFlowerState(position));
    blocks.fire_flowers.back().is_visible = false;

    put_qblock(position);
  }

  using Direction = EntityState::Direction;
  static constexpr auto DirectionLeft = EntityState::DirectionLeft;

  template<typename Entity>
  auto put_entity(
      std::vector<Entity>& array, 
      const glm::vec2& position,
      const glm::vec2& size,
      Direction direction,
      int reward,
      int walk_speed,
      Texture* texture
  ){
    auto entity = Entity();

    entity.position = position * config::BlockSize;
    entity.size = size;
    entity.current_texture = texture;
    entity.set_direction(direction, walk_speed);

    array.push_back(entity);
  }

  auto put_goomba(const glm::vec2& position, Direction direction = DirectionLeft){
    put_entity(
      entities.goombas, 
      position, 
      glm::vec2(config::BlockSize),
      direction, 
      config::RewardForKillingGoomba, 
      config::GoombaWalkSpeed,
      &textures::goomba_walk[0]
    );
  }

  auto put_red_goomba(
      const glm::vec2& position,
      EntityState::Direction direction = EntityState::DirectionLeft,
      bool instantly_active = false
  ){
    put_entity(
      entities.red_goombas, 
      position, 
      glm::vec2(config::BlockSize),
      direction, 
      config::RewardForKillingGoomba, 
      config::GoombaWalkSpeed,
      &textures::red_goomba_walk[0]
    );

    entities.red_goombas.back().fall_from_edge = false;
    entities.red_goombas.back().is_active = instantly_active;
  }
  
  auto put_yellow_goomba(const glm::vec2& position, EntityState::Direction direction = EntityState::DirectionLeft){
    put_entity(
      entities.yellow_goombas, 
      position, 
      glm::vec2(config::BlockSize),
      direction, 
      config::RewardForKillingGoomba, 
      config::FastGoombaWalkSpeed,
      &textures::yellow_goomba_walk[0]
    );

    entities.yellow_goombas.back().fall_from_edge = false;
    entities.yellow_goombas.back().is_active = true;
  }

  auto put_mushroom(const glm::vec2& position, EntityState::Direction direction = EntityState::DirectionLeft){
    put_entity(
      entities.mushrooms,
      position,
      glm::vec2(config::BlockSize),
      direction,
      config::RewardForEatingMushroom,
      config::MushroomWalkSpeed,
      &textures::mushroom
    );

    entities.mushrooms.back().is_visible = false;
  }

  auto put_green_koopa(const glm::vec2& position, Direction direction = DirectionLeft){
    put_entity(
      entities.green_koopas,
      position,
      glm::vec2(config::BlockSize, config::BlockSize * 1.5f),
      direction,
      0,
      config::KoopaWalkSpeed,
      &textures::green_koopa_walk[0]
    );

    auto& koopa = entities.green_koopas.back();
    koopa.texture_flip = Drawable::Flip::UseFlip;
  }

  auto put_red_koopa(const glm::vec2& position, Direction direction = DirectionLeft){
    put_entity(
      entities.red_koopas,
      position,
      glm::vec2(config::BlockSize, config::BlockSize * 1.5f),
      direction,
      0,
      config::KoopaWalkSpeed,
      &textures::red_koopa_walk[0]
    );

    auto& koopa = entities.red_koopas.back();
    koopa.texture_flip = Drawable::Flip::UseFlip;
    koopa.fall_from_edge = false;
  }

  auto put_beetle(const glm::vec2& position, Direction direction = DirectionLeft){
    put_entity(
      entities.beetles,
      position,
      glm::vec2(config::BlockSize),
      direction,
      0,
      config::BeetleWalkSpeed,
      &textures::beetle_walk[0]
    );

    auto& beetle = entities.beetles.back();
    beetle.texture_flip = Drawable::Flip::UseFlip;
  }

  auto put_qblock_with_mushroom(const glm::vec2& position, Direction direction = DirectionLeft){
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
