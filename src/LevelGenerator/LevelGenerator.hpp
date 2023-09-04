#pragma once
#include "LevelGenerator/Tiles.hpp"
#include "LevelGenerator/Builder.hpp"
#include "LevelGenerator/Textures.hpp"

#include "States/JumpingKoopaState.hpp"
#include "States/LevelState.hpp"
#include "Util/Util.hpp"
#include "config.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <unordered_set>

namespace mario::level_generator{

static auto generate_horizontal_level_clouds(LevelState& level, int spread = 2){
  auto& clouds = level.game_objects.get_vec<CloudState>();
  clouds.reserve(64);

  for (int i = 0; i < 16; ++i){
    clouds.push_back({ glm::vec2(i * 18 + 1, spread / 2 + 1), 3 });
    clouds.push_back({ glm::vec2(i * 18 + 7, 2), 2 });
    clouds.push_back({ glm::vec2(i * 18 + 12, spread + 1), 1 });
    clouds.push_back({ glm::vec2(i * 18 + 15, spread / 3 + 1), 1 });
  }
}

static auto generate_vertical_level_clouds(LevelState& level){
  auto& clouds = level.game_objects.get_vec<CloudState>();

  static constexpr auto CloudRows = 11;
  clouds.reserve(CloudRows * 2 * 2 * 3);

  for (int j = 0; j < CloudRows; ++j){
    for (int i = 0; i < 2; ++i){
      clouds.push_back({ glm::vec2(i * 18 + 4, 2 + j * 14), 2 });
      clouds.push_back({ glm::vec2(i * 18 + 9, 3 + j * 14), 3 });
      clouds.push_back({ glm::vec2(i * 18 + 15, 1 + j * 14), 1 });
    }

    for (int i = 0; i < 2; ++i){
      clouds.push_back({ glm::vec2(i * 18 + 4 - 2, 2 + j * 14 + 7), 2 });
      clouds.push_back({ glm::vec2(i * 18 + 9 - 2, 3 + j * 14 + 7), 3 });
      clouds.push_back({ glm::vec2(i * 18 + 15 - 2, 1 + j * 14 + 7), 1 });
    }
  }
}

static auto generate_level(LevelState& level, const std::string& file_path){
  const auto level_file = config::LevelsSourceDir + file_path; 

  auto file = std::ifstream(level_file);
  if (!file) throw std::runtime_error("Error while loading level from: " + level_file);

  auto file_content = util::get_file_content(level_file);
  std::replace(file_content.begin(), file_content.end(), ',', ' ');

  auto ss = std::istringstream(file_content);
  auto tile_id = 0;
  auto counter = 0;

  auto& objects = level.game_objects;

  while(ss >> tile_id){
    const auto x = counter % (level.max_size().x | util::as<int>);
    const auto y = counter / (level.max_size().x | util::as<int>);

    counter++;

    const auto tile = tile_id | util::as<level_generator::Tile>;

    if (tile_id >= 0){
      allocated_textures.insert(tile);
      level.min_scroll_y = std::min(y, level.min_scroll_y);
    }


    if (tile == Tile::MushroomBot2) put_nonsolid(level, { x, y }, textures::mushroom_bot2);
    else if (tile == Tile::MushroomBot1) put_nonsolid(level, { x, y }, textures::mushroom_bot1);
    else if (tile == Tile::GrassBot) put_nonsolid(level, { x, y }, textures::grass_bot);
    else if (tile == Tile::SnowBot) put_nonsolid(level, { x, y }, textures::snow_bot);
    else if (tile == Tile::SmallStone) put_nonsolid(level, { x, y }, textures::small_stone);
    else if (tile == Tile::Bricks) put_bricks(level, BricksBlockState(texture_groups::bricks, { x, y }));
    else if (tile == Tile::BlueBricks) put_bricks(level, BricksBlockState(texture_groups::blue_bricks, { x, y }));

    else if (any_tile(HillTiles, tile)) put_hill(level, { x, y }, tile);
    else if (any_tile(BushTiles, tile)) put_bush(level, { x, y }, tile);

    else if (tile == Tile::Coin) objects.push(CoinBlockState({ x, y }));

    else if (tile == Tile::QBlockMushroom) put_q_block_with_mushroom(level, { x, y }, MushroomState::Type::Red);
    else if (tile == Tile::QBlockGreenMushroom) put_q_block_with_mushroom(level, { x, y }, MushroomState::Type::Green);
    else if (tile == Tile::QBlockCoins) put_q_block_with_coins(level, { x, y });
    else if (tile == Tile::QBlockFireFlower) put_q_block_with_flower(level, { x, y });

    //Entities:    
    else if (tile == Tile::YellowGoomba) objects.push(GoombaState::make_yellow({ x, y }));
    else if (tile == Tile::RedGoomba) objects.push(GoombaState::make_red({ x, y }));
    else if (tile == Tile::Goomba) objects.push(GoombaState::make_normal({ x, y }));
    else if (tile == Tile::RedKoopa) objects.push(KoopaState::make_red({ x, y }));
    else if (tile == Tile::GreenKoopa) objects.push(KoopaState::make_green({ x, y }));
    else if (tile == Tile::PurpleKoopa) objects.push(KoopaState::make_purple({ x, y }));
    else if (tile == Tile::RedKoopaWings) objects.push(JumpingKoopaState::make_red({ x, y }));
    else if (tile == Tile::GreenKoopaWings) objects.push(JumpingKoopaState::make_green({ x, y }));
    else if (tile == Tile::PurpleKoopaWings) objects.push(JumpingKoopaState::make_purple({ x, y }));
    else if (tile == Tile::GreenPlant) objects.push(PlantState::make_green({ x, y }));
    else if (tile == Tile::RedPlant) objects.push(PlantState::make_red({ x, y }));
    else if (tile == Tile::BlackPlant) objects.push(BlackPlantState::make({ x, y }));
    else if (tile == Tile::Hammerbro) objects.push(HammerBroState::make({ x, y }));
    else if (tile == Tile::RedHammerbro) objects.push(HammerBroState::make_red({ x, y }));
    else if (tile == Tile::Beetle) objects.push(BeetleState::make({ x, y }));
    else if (tile == Tile::Spike) objects.push(SpikeState::make({ x, y }));
    else if (tile == Tile::GreyFish) objects.push(FishState::make_grey({ x, y }));
    else if (tile == Tile::RedFish) objects.push(FishState::make_red({ x, y }));
    else if (tile == Tile::Squid) objects.push(SquidState::make({ x, y }));

    else if (tile == Tile::Hitbox) put_hitbox_block(level, { x, y });
    else if (tile == Tile::Finish) level.finish_position = { x, y };
    
    else if (tile == Tile::Checkpoint) put_checkpoint(level, { x, y });
    else if (&id_to_texture.at(tile).front() != level_generator::no_texture || tile == Tile::Dirt) {
      put_solid(level, { x, y }, id_to_texture.at(tile).front());
    }
  }
}

} //namespace mario::level_generator
