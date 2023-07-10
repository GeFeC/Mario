#pragma once

#include "LevelGenerator/Textures.hpp"
#include "States/BlockState.hpp"
#include "States/FireFlowerPusherState.hpp"
#include "States/FireFlowerState.hpp"
#include "States/JumpingKoopaState.hpp"
#include "States/LevelState.hpp"
#include <glm/glm.hpp>

namespace mario::level_generator{

using renderer::Texture;

static auto put_solid(LevelState& level, const glm::vec2& position, const Texture& texture){
  level.hitbox_grid_element(position) = 1;
  return level.game_objects.push(BlockState(position, &texture));
}

static auto put_hitbox_block(LevelState& level, const glm::vec2& position){
  put_solid(level, position, textures::dirt);
  level.game_objects.back<BlockState>().is_visible = false;
}

static auto put_nonsolid(LevelState& level, const glm::vec2& position, const Texture& texture){
  auto& block = level.game_objects.push(BlockState(position, &texture));
  block.is_solid = false;
}

static auto put_bricks(LevelState& level, const BricksBlockState& bricks){
  const auto& position = bricks.position / BlockBase::Size;

  level.hitbox_grid_element(position) = 1;
  put_solid(level, position, textures::dirt);
  level.game_objects.back<BlockState>().is_visible = false;

  level.game_objects.push(bricks);
  level.game_objects.back<BricksBlockState>().hitbox_block_index = level.game_objects.size<BlockState>() - 1;
};

static auto put_q_block_with_coins(LevelState& level, const glm::vec2& position, int coins = 1){
  put_hitbox_block(level, position);
  auto& block = level.game_objects.push(QBlockState<CoinPusherState>(position));
  block.bounce_state.bounces_count = coins;
  block.pusher.coins.resize(coins, SpinningCoinState(position));

  auto& points = block.pusher.points_generator.items;
  points.reserve(coins);

  for (int i = 0; i < coins; ++i){
    auto particle = PointsParticlesState(QBlockReward, position * BlockBase::Size);

    points.push_back(particle);
  }
}

static auto put_q_block_with_flower(LevelState& level, const glm::vec2& position){
  put_hitbox_block(level, position);

  auto& block = level.game_objects.push(QBlockState<FireFlowerPusherState>(position));
  block.pusher.fire_flower = FireFlowerState(position);
}

using Direction = EntityState::Direction;
static constexpr auto DirectionLeft = EntityState::DirectionLeft;

static auto put_q_block_with_mushroom(
    LevelState& level, 
    const glm::vec2& position,
    MushroomState::Type mushroom_type
){
  put_hitbox_block(level, position);
  auto& block = level.game_objects.push(QBlockState<MushroomPusherState>(position));
  auto& mushroom = block.pusher.entity;

  using MushroomType = MushroomState::Type;
  switch(mushroom_type){
    case MushroomType::Red: mushroom = MushroomState::make_red(position); break;
    case MushroomType::Green: mushroom = MushroomState::make_green(position); break;
  }
}

static auto put_checkpoint(LevelState& level, const glm::vec2& position){
  level.checkpoints.push_back(position * BlockBase::Size);
}

static auto put_hill(LevelState& level, const glm::vec2& position, Tile tile){
  level.game_objects.push(BackgroundHillState(position, &id_to_texture[tile].front()));
}

static auto put_bush(LevelState& level, const glm::vec2& position, Tile tile){
  level.game_objects.push(BackgroundBushState(position, &id_to_texture[tile].front()));
}

//Entities:
template<typename Entity>
static auto& put_q_block_with_entity(
    LevelState& level, 
    const Entity& entity
){
  const auto block_position = (entity.position + glm::vec2(0.f, entity.size.y - BlockBase::Size)) / BlockBase::Size;

  level_generator::put_hitbox_block(level, block_position);

  auto& block = level.game_objects.push(QBlockState<EntityPusherState<Entity>>(block_position));
  auto& pusher_entity = block.pusher.entity;

  pusher_entity = entity;
  pusher_entity.is_active = false;
  pusher_entity.is_in_q_block = true;
  pusher_entity.is_visible = false;

  return block;
}

} //namespace mario::level_generator
