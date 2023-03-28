#pragma once
#include "States/JumpingKoopaState.hpp"
#include "States/LevelState.hpp"
#include "Util/Util.hpp"
#include "config.hpp"
#include "res/textureGroups.hpp"

#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <unordered_set>

namespace level_generator{

using Direction = EntityState::Direction;
static constexpr auto DirectionLeft = EntityState::DirectionLeft;

static auto generate_horizontal_level_clouds(LevelState& level, int spread = 2){
  auto& clouds = level.background.clouds;
  clouds.reserve(64);

  for (int i = 0; i < 16; ++i){
    clouds.push_back(std::make_pair(glm::vec2(i * 18 + 1, spread / 2), 3));
    clouds.push_back(std::make_pair(glm::vec2(i * 18 + 7, 1), 2));
    clouds.push_back(std::make_pair(glm::vec2(i * 18 + 12, spread), 1));
    clouds.push_back(std::make_pair(glm::vec2(i * 18 + 15, spread / 3), 1));
  }
}

static auto generate_vertical_level_clouds(LevelState& level){
  auto& clouds = level.background.clouds;
  clouds.reserve(14 * 2 * 2 * 3);

  for (int j = 0; j < 14; ++j){
    for (int i = 0; i < 2; ++i){
      clouds.push_back(std::make_pair(glm::vec2(i * 18 + 4, 2 + j * 14), 2));
      clouds.push_back(std::make_pair(glm::vec2(i * 18 + 9, 3 + j * 14), 3));
      clouds.push_back(std::make_pair(glm::vec2(i * 18 + 15, 1 + j * 14), 1));
    }

    for (int i = 0; i < 2; ++i){
      clouds.push_back(std::make_pair(glm::vec2(i * 18 + 4 - 2, 2 + j * 14 + 7), 2));
      clouds.push_back(std::make_pair(glm::vec2(i * 18 + 9 - 2, 3 + j * 14 + 7), 3));
      clouds.push_back(std::make_pair(glm::vec2(i * 18 + 15 - 2, 1 + j * 14 + 7), 1));
    }
  }
}

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

static auto index_to_texture = std::vector<TextureGroup>{
  &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
  &textures::mushroom_right,
  &textures::mushroom_left,
  &textures::mushroom_center,
  &textures::mushroom_bot2,
  &textures::mushroom_bot1,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
  texture_groups::bricks,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
  texture_groups::q_block,  
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
  &textures::red_pipe_top_right,
  &textures::red_pipe_bottom_right,
  &textures::red_pipe_top_left,
  &textures::red_pipe_bottom_left,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
  &textures::red_hill_top, 
  &textures::red_hill_right,
  &textures::red_hill_left,
  &textures::red_hill_center,
  &textures::red_hill_center_dot,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
    &textures::dirt,
  texture_groups::yellow_goomba,
  texture_groups::red_goomba,
  texture_groups::goomba,
  texture_groups::spike,
  texture_groups::plant,
  texture_groups::red_plant,
  texture_groups::red_koopa,
  texture_groups::red_flying_koopa,
  texture_groups::green_koopa,
  texture_groups::green_flying_koopa,
  texture_groups::hammerbro,
  texture_groups::beetle,
    &textures::dirt,
  &textures::red_bush_right,
  &textures::red_bush_left,
  &textures::red_bush_center,
    &textures::dirt,
    &textures::dirt,
  texture_groups::coin,
  &textures::mushroom,
  &textures::green_mushroom,
  texture_groups::spinning_coin,
  texture_groups::fire_flower,
    &textures::dirt,
    &textures::dirt
};

static auto allocate_texture_if_needed(int texture_id){
  index_to_texture[texture_id].allocate();
}

static auto allocated_textures = std::unordered_set<int>{};

static auto generate_level(LevelState& level, const std::string& file_path){
  const auto level_file = config::LevelsSourceDir + file_path; 
  auto file = std::ifstream(level_file);
  if (!file) throw std::runtime_error("Error while loading level from: " + level_file);

  auto file_content = util::get_file_content(level_file);
  std::replace(file_content.begin(), file_content.end(), ',', ' ');

  auto ss = std::istringstream(file_content);
  auto tile_id = 0;
  auto counter = 0;

  while(ss >> tile_id){
    if (tile_id >= 0){
      allocated_textures.insert(tile_id);
    }

    const auto x = counter % (level.get_size().x | util::as<int>);
    const auto y = counter / (level.get_size().x | util::as<int>);

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
    if (tile_id == 65) entities.green_jumping_koopas.push_back(JumpingKoopaState::make_green({ x, y - 0.5f }));
    if (tile_id == 66) entities.hammerbros.push_back(HammerBroState::make({ x, y - 1.f }));

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
    if (tile_id == 79) { put_solid(level, { x, y }, textures::dirt); level.blocks.normal.back().is_visible = false; }
    if (tile_id == 80) level.checkpoints.push_back({ x * config::BlockSize, y * config::BlockSize });
    
    counter++;
  }
}

} //namespace level_generator
