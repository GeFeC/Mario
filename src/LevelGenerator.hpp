#pragma once
#include "States/JumpingKoopaState.hpp"
#include "States/LevelState.hpp"
#include "Util/Util.hpp"
#include "config.hpp"

#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iostream>


namespace level_generator{

using Direction = EntityState::Direction;
static constexpr auto DirectionLeft = EntityState::DirectionLeft;

static auto put_solid(LevelState& level, const glm::vec2& position, const Texture& texture){
  level.get_hitbox_grid_element(position) = 1;
  level.blocks.normal.push_back(BlockState(position, &texture));
}

static auto put_nonsolid(LevelState& level, const glm::vec2& position, const Texture& texture){
  level.blocks.normal.push_back(BlockState(position, &texture));
  level.blocks.normal.back().is_solid = false;
}

auto put_bricks(LevelState& level, const glm::vec2& position){
  level.get_hitbox_grid_element(position) = 1;

  put_solid(level, position, textures::dirt);
  level.blocks.normal.back().is_visible = false;
  level.blocks.bricks.push_back(BricksBlockState(position));
  level.blocks.bricks.back().hitbox_block_index = level.blocks.normal.size() - 1;
};

auto put_mushroom_head(LevelState& level, const glm::vec2& position, int size){
  level.get_hitbox_grid_element(position) = 1;

  level.blocks.normal.push_back(BlockState(position, &textures::mushroom_left));

  for (int i = 0; i < size; ++i){
    level.get_hitbox_grid_element(position + glm::vec2(i + 1, 0)) = 1;
    level.blocks.normal.push_back(BlockState(position + glm::vec2(i + 1, 0), &textures::mushroom_center));
  }

  level.get_hitbox_grid_element(position + glm::vec2(size + 1, 0)) = 1;
  level.blocks.normal.push_back(BlockState(position + glm::vec2(size + 1, 0), &textures::mushroom_right));
}

auto put_coin(LevelState& level, const glm::vec2& position){
  level.blocks.coins.push_back(CoinBlockState(position));
}

auto put_spinning_coin(LevelState& level, const glm::vec2& position, int hits_required_to_bounce){
  level.blocks.spinning_coins.push_back(SpinningCoinState(position));

  level.blocks.spinning_coins.back().bounce_state.initial_power = -20.f;
  level.blocks.spinning_coins.back().bounce_state.hits_required_to_bounce = hits_required_to_bounce;
  
  if (hits_required_to_bounce > 0){
    level.blocks.spinning_coins.back().is_visible = false;
  }
}

auto put_qblock(LevelState& level, const glm::vec2& position, int bounces = 1){
  put_solid(level, position, textures::dirt);
  level.blocks.normal.back().is_visible = false;

  level.blocks.q_blocks.push_back(QBlockState(position));
  level.blocks.q_blocks.back().bounce_state.bounces_count = bounces;
}

auto put_qblock_with_coins(LevelState& level, const glm::vec2& position, int coins = 1){
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

static auto put_qblock_with_mushroom(LevelState& level, const glm::vec2& position, Direction direction = DirectionLeft){
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

auto put_red_pipe(LevelState& level, const glm::vec2& position, int size){
  for (int i = 0; i < size; ++i){
    level.blocks.normal.push_back(BlockState({ position.x, position.y - i }, &textures::red_pipe_bottom_left));
    level.blocks.normal.push_back(BlockState({ position.x + 1, position.y - i }, &textures::red_pipe_bottom_right));
  }

  level.blocks.normal.push_back(BlockState({ position.x, position.y - size }, &textures::red_pipe_top_left));
  level.blocks.normal.push_back(BlockState({ position.x + 1, position.y - size }, &textures::red_pipe_top_right));
}


static auto generate_level(LevelState& level, const std::string& level_file){
  auto file = std::ifstream(level_file);
  if (!file) throw std::runtime_error("Error while loading level from: " + level_file);

  auto file_content = util::get_file_content(level_file);
  std::replace(file_content.begin(), file_content.end(), ',', ' ');

  auto ss = std::istringstream(file_content);
  auto tile_id = 0;
  auto counter = 0;
  while(ss >> tile_id){
    const auto x = counter % config::MaxLevelSize;
    const auto y = counter / config::MaxLevelSize;

    auto& entities = level.entities;
    using config::BlockSize;
    if (tile_id == 0) put_solid(level, { x, y }, textures::dirt);
    if (tile_id == 5) put_solid(level, { x, y }, textures::mushroom_right);
    if (tile_id == 6) put_solid(level, { x, y }, textures::mushroom_left);
    if (tile_id == 7) put_solid(level, { x, y }, textures::mushroom_center);
    if (tile_id == 8) put_nonsolid(level, { x, y }, textures::mushroom_bot2);
    if (tile_id == 9) put_nonsolid(level, { x, y }, textures::mushroom_bot1);
    if (tile_id == 16) put_bricks(level, { x, y });

    if (tile_id == 31) put_solid(level, { x, y }, textures::red_pipe_top_right);
    if (tile_id == 32) put_solid(level, { x, y }, textures::red_pipe_bottom_right);
    if (tile_id == 33) put_solid(level, { x, y }, textures::red_pipe_top_left);
    if (tile_id == 34) put_solid(level, { x, y }, textures::red_pipe_bottom_left);

    if (tile_id == 47) level.background.hills.emplace_back(glm::vec2(x, y), &textures::red_hill_top);
    if (tile_id == 48) level.background.hills.emplace_back(glm::vec2(x, y), &textures::red_hill_right);
    if (tile_id == 49) level.background.hills.emplace_back(glm::vec2(x, y), &textures::red_hill_left);
    if (tile_id == 50) level.background.hills.emplace_back(glm::vec2(x, y), &textures::red_hill_center);
    if (tile_id == 51) level.background.hills.emplace_back(glm::vec2(x, y), &textures::red_hill_center_dot);

    if (tile_id == 60) entities.plants.push_back(PlantState::make_green({ x + 0.5f, y + 1 }));
    if (tile_id == 63) entities.red_jumping_koopas.push_back(JumpingKoopaState::make_red({ x, y - 0.5f }));

    if (tile_id == 69) level.background.bushes.emplace_back(glm::vec2(x, y), &textures::red_bush_right);
    if (tile_id == 70) level.background.bushes.emplace_back(glm::vec2(x, y), &textures::red_bush_left);
    if (tile_id == 71) level.background.bushes.emplace_back(glm::vec2(x, y), &textures::red_bush_center);

    if (tile_id == 56) entities.yellow_goombas.push_back(GoombaState::make_yellow({ x, y }));
    if (tile_id == 57) entities.red_goombas.push_back(GoombaState::make_red({ x, y }));
    if (tile_id == 58) entities.goombas.push_back(GoombaState::make_normal({ x, y }));
    if (tile_id == 62) entities.red_koopas.push_back(KoopaState::make_red({ x, y - 0.5f }));
    if (tile_id == 64) entities.green_koopas.push_back(KoopaState::make_green({ x, y - 0.5f }));
    if (tile_id == 74) put_coin(level, { x, y });
    if (tile_id == 75) put_qblock_with_mushroom(level, { x, y });
    if (tile_id == 76) put_qblock_with_green_mushroom(level, { x, y });
    if (tile_id == 77) put_qblock_with_coins(level, { x, y });
    if (tile_id == 78) put_qblock_with_flower(level, { x, y });
    
    counter++;
  }
}

} //namespace level_generator
