#pragma once

#include "LevelGenerator/Textures.hpp"
#include "States/BlockState.hpp"
#include "States/FireFlowerState.hpp"
#include "States/JumpingKoopaState.hpp"
#include "States/LevelState.hpp"
#include <glm/glm.hpp>

namespace level_generator{

static auto put_solid(LevelState& level, const glm::vec2& position, const Texture& texture){
  level.hitbox_grid_element(position) = 1;
  level.blocks.normal.push_back(BlockState(position, &texture));
}

static auto put_hitbox_block(LevelState& level, const glm::vec2& position){
  put_solid(level, position, textures::dirt);
  level.blocks.normal.back().is_visible = false;
}

static auto put_nonsolid(LevelState& level, const glm::vec2& position, const Texture& texture){
  level.blocks.normal.push_back(BlockState(position, &texture));
  level.blocks.normal.back().is_solid = false;
}

static auto put_bricks(LevelState& level, const glm::vec2& position){
  level.hitbox_grid_element(position) = 1;

  put_solid(level, position, textures::dirt);
  level.blocks.normal.back().is_visible = false;
  level.blocks.bricks.push_back(BricksBlockState(position));
  level.blocks.bricks.back().hitbox_block_index = level.blocks.normal.size() - 1;
};

static auto put_coin(LevelState& level, const glm::vec2& position){
  level.blocks.coins.push_back(CoinBlockState(position));
}

static auto put_qblock_with_coins(LevelState& level, const glm::vec2& position, int coins = 1){
  put_hitbox_block(level, position);
  auto& block = level.blocks.q_blocks_with_coins.emplace_back(position);
  block.bounce_state.bounces_count = coins;
  block.pusher.coins.resize(coins, SpinningCoinState(position));

  auto& points = level.blocks.q_blocks_with_coins.back().pusher.points_generator.items;
  points.reserve(coins);

  for (int i = 0; i < coins; ++i){
    auto particle = PointsParticlesState(QBlockReward, position * BlockBase::Size);

    points.push_back(particle);
  }
}

static auto put_qblock_with_flower(LevelState& level, const glm::vec2& position){
  put_hitbox_block(level, position);

  auto& block = level.blocks.q_blocks_with_flower.emplace_back(position);
  block.pusher.fire_flower = FireFlowerState(position);
}

using Direction = EntityState::Direction;
static constexpr auto DirectionLeft = EntityState::DirectionLeft;

static auto put_qblock_with_mushroom(
    LevelState& level, 
    const glm::vec2& position,
    MushroomState::Type mushroom_type
){
  put_hitbox_block(level, position);
  auto& block = level.blocks.q_blocks_with_mushroom.emplace_back(position);
  auto& mushroom = block.pusher.entity;

  mushroom.type = mushroom_type;

  using MushroomType = MushroomState::Type;
  mushroom = [&]{
    switch(mushroom.type){
      case MushroomType::Red: return MushroomState::make_red(position);
      case MushroomType::Green: return MushroomState::make_green(position);
    }
  }();
}

static auto put_checkpoint(LevelState& level, const glm::vec2& position){
  level.checkpoints.push_back(position * BlockBase::Size);
}

static auto put_hill(LevelState& level, const glm::vec2& position, Tile tile){
  level.background.hills.emplace_back(position, &id_to_texture[tile].front());
}

static auto put_bush(LevelState& level, const glm::vec2& position, Tile tile){
  level.background.bushes.emplace_back(position, &id_to_texture[tile].front());
}

//Entities:

static auto put_goomba(LevelState& level, const glm::vec2& position){
  level.entities.goombas.push_back(GoombaState::make_normal(position));
}

static auto put_red_goomba(LevelState& level, const glm::vec2& position){
  level.entities.red_goombas.push_back(GoombaState::make_red(position));
}

static auto put_yellow_goomba(LevelState& level, const glm::vec2& position){
  level.entities.yellow_goombas.push_back(GoombaState::make_yellow(position));
}

static auto put_green_koopa(LevelState& level, const glm::vec2& position){
  level.entities.green_koopas.push_back(KoopaState::make_green(position - glm::vec2(0, 0.5)));
}

static auto put_red_koopa(LevelState& level, const glm::vec2& position){
  level.entities.red_koopas.push_back(KoopaState::make_red(position - glm::vec2(0, 0.5)));
}

static auto put_purple_koopa(LevelState& level, const glm::vec2& position){
  level.entities.purple_koopas.push_back(KoopaState::make_purple(position - glm::vec2(0, 0.5)));
}

static auto put_green_koopa_with_wings(LevelState& level, const glm::vec2& position){
  level.entities.green_jumping_koopas.push_back(JumpingKoopaState::make_green(position - glm::vec2(0, 0.5)));
}

static auto put_red_koopa_with_wings(LevelState& level, const glm::vec2& position){
  level.entities.red_jumping_koopas.push_back(JumpingKoopaState::make_red(position - glm::vec2(0, 0.5)));
}

static auto put_purple_koopa_with_wings(LevelState& level, const glm::vec2& position){
  level.entities.purple_jumping_koopas.push_back(JumpingKoopaState::make_purple(position - glm::vec2(0, 0.5)));
}

static auto put_plant(LevelState& level, const glm::vec2& position){
  level.entities.plants.push_back(PlantState::make_green(position + glm::vec2(0.5, 1.f)));
}

static auto put_red_plant(LevelState& level, const glm::vec2& position){
  level.entities.red_plants.push_back(PlantState::make_red(position + glm::vec2(0.5, 1.f)));
}

static auto put_hammerbro(LevelState& level, const glm::vec2& position){
  level.entities.hammerbros.push_back(HammerBroState::make(position - glm::vec2(0, 1)));
}

static auto put_beetle(LevelState& level, const glm::vec2& position){
  level.entities.beetles.push_back(BeetleState::make(position - glm::vec2(0, 0.5)));
}

template<typename Function>
static auto put_q_block_with_entity(LevelState& level, const glm::vec2& position, Function make_entity){
  level_generator::put_hitbox_block(level, position);
  auto& block = level.blocks.q_blocks_with_goomba.emplace_back(position);
  block.pusher.entity = make_entity(position);

  block.pusher.entity.is_in_q_block = true;
  block.pusher.entity.is_visible = false;
}

} //namespace level_generator
