#pragma once

#include "LevelGenerator/Textures.hpp"
#include "States/JumpingKoopaState.hpp"
#include "States/LevelState.hpp"
#include <glm/glm.hpp>

namespace level_generator{

static auto put_solid(LevelState& level, const glm::vec2& position, const Texture& texture){
  level.get_hitbox_grid_element(position) = 1;
  level.blocks.normal.push_back(BlockState(position, &texture));
}

static auto put_nonsolid(LevelState& level, const glm::vec2& position, const Texture& texture){
  level.blocks.normal.push_back(BlockState(position, &texture));
  level.blocks.normal.back().is_solid = false;
}

static auto put_bricks(LevelState& level, const glm::vec2& position){
  level.get_hitbox_grid_element(position) = 1;

  put_solid(level, position, textures::dirt);
  level.blocks.normal.back().is_visible = false;
  level.blocks.bricks.push_back(BricksBlockState(position));
  level.blocks.bricks.back().hitbox_block_index = level.blocks.normal.size() - 1;
};

static auto put_coin(LevelState& level, const glm::vec2& position){
  level.blocks.coins.push_back(CoinBlockState(position));
}

static auto put_spinning_coin(LevelState& level, const glm::vec2& position, int hits_required_to_bounce){
  level.blocks.spinning_coins.push_back(SpinningCoinState(position));

  level.blocks.spinning_coins.back().bounce_state.initial_power = -20.f;
  level.blocks.spinning_coins.back().bounce_state.hits_required_to_bounce = hits_required_to_bounce;
  
  if (hits_required_to_bounce > 0){
    level.blocks.spinning_coins.back().is_visible = false;
  }
}

static auto put_qblock(LevelState& level, const glm::vec2& position, int bounces = 1){
  put_solid(level, position, textures::dirt);
  level.blocks.normal.back().is_visible = false;

  level.blocks.q_blocks.push_back(QBlockState(position));
  level.blocks.q_blocks.back().bounce_state.bounces_count = bounces;
}

static auto put_qblock_with_coins(LevelState& level, const glm::vec2& position, int coins = 1){
  for (int i = 0; i < coins; ++i){
    put_spinning_coin(level, position, i + 1);
  }

  put_qblock(level, position, coins);

  auto& points = level.blocks.q_blocks.back().points_generator.items;
  points.reserve(coins);
  for (int i = 0; i < coins; ++i){
    auto particle = PointsParticlesState(QBlockState::RewardInPoints, position * config::BlockSize);
    particle.hits_required_to_activate = i + 1;

    points.push_back(particle);
  }
}

static auto put_qblock_with_flower(LevelState& level, const glm::vec2& position){
  level.blocks.fire_flowers.push_back(FireFlowerState(position));
  level.blocks.fire_flowers.back().is_visible = false;

  put_qblock(level, position);
}

using Direction = EntityState::Direction;
static constexpr auto DirectionLeft = EntityState::DirectionLeft;

static auto put_qblock_with_mushroom(
    LevelState& level, 
    const glm::vec2& position, 
    Direction direction = DirectionLeft
){
  put_qblock(level, position, 1);
  level.entities.mushrooms.push_back(MushroomState::make_red(position, direction));
}

static auto put_qblock_with_green_mushroom(
    LevelState& level, 
    const glm::vec2& position, 
    Direction direction = DirectionLeft
){
  put_qblock(level, position, 1);
  level.entities.green_mushrooms.push_back(MushroomState::make_green(position, direction));
}

static auto put_hitbox_block(LevelState& level, const glm::vec2& position){
  put_solid(level, position, textures::dirt);
  level.blocks.normal.back().is_visible = false;
}

static auto put_checkpoint(LevelState& level, const glm::vec2& position){
  level.checkpoints.push_back(position * config::BlockSize);
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

static auto put_hammerbro(LevelState& level, const glm::vec2& position){
  level.entities.hammerbros.push_back(HammerBroState::make(position - glm::vec2(0, 1)));
}

} //namespace level_generator
