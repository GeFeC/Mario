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

namespace level_generator{

static auto generate_horizontal_level_clouds(LevelState& level, int spread = 2){
  auto& clouds = level.background.clouds;
  clouds.reserve(64);

  for (int i = 0; i < 16; ++i){
    clouds.push_back(std::make_pair(glm::vec2(i * 18 + 1, spread / 2 + 1), 3));
    clouds.push_back(std::make_pair(glm::vec2(i * 18 + 7, 2), 2));
    clouds.push_back(std::make_pair(glm::vec2(i * 18 + 12, spread + 1), 1));
    clouds.push_back(std::make_pair(glm::vec2(i * 18 + 15, spread / 3 + 1), 1));
  }
}

static auto generate_vertical_level_clouds(LevelState& level){
  auto& clouds = level.background.clouds;

  static constexpr auto CloudRows = 11;
  clouds.reserve(CloudRows * 2 * 2 * 3);

  for (int j = 0; j < CloudRows; ++j){
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
    const auto tile = tile_id | util::as<level_generator::Tile>;

    if (tile_id >= 0){
      allocated_textures.insert(tile);
    }

    const auto x = counter % (level.max_size().x | util::as<int>);
    const auto y = counter / (level.max_size().x | util::as<int>);

    auto& entities = level.entities;

    if (tile == Tile::MushroomBot2) put_nonsolid(level, { x, y }, textures::mushroom_bot2);
    else if (tile == Tile::MushroomBot1) put_nonsolid(level, { x, y }, textures::mushroom_bot1);
    else if (tile == Tile::GrassBot) put_nonsolid(level, { x, y }, textures::grass_bot);
    else if (tile == Tile::SnowBot) put_nonsolid(level, { x, y }, textures::snow_bot);
    else if (tile == Tile::Bricks) put_bricks(level, { x, y });

    else if (any_tile(HillTiles, tile)) put_hill(level, { x, y }, tile);
    else if (any_tile(BushTiles, tile)) put_bush(level, { x, y }, tile);

    //Entities:    
    else if (tile == Tile::YellowGoomba) put_yellow_goomba(level, { x, y });
    else if (tile == Tile::RedGoomba) put_red_goomba(level, { x, y });
    else if (tile == Tile::Goomba) put_goomba(level, { x, y });
    else if (tile == Tile::RedKoopa) put_red_koopa(level, { x, y });
    else if (tile == Tile::GreenKoopa) put_green_koopa(level, { x, y });
    else if (tile == Tile::PurpleKoopa) put_purple_koopa(level, { x, y });
    else if (tile == Tile::RedKoopaWings) put_red_koopa_with_wings(level, { x, y });
    else if (tile == Tile::GreenKoopaWings) put_green_koopa_with_wings(level, { x, y });
    else if (tile == Tile::PurpleKoopaWings) put_purple_koopa_with_wings(level, { x, y });
    else if (tile == Tile::GreenPlant) put_plant(level, { x, y });
    else if (tile == Tile::RedPlant) put_red_plant(level, { x, y });
    else if (tile == Tile::Hammerbro) put_hammerbro(level, { x, y });
    else if (tile == Tile::Beetle) put_beetle(level, { x, y });

    else if (tile == Tile::Coin) put_coin(level, { x, y });

    else if (tile == Tile::QBlockMushroom) put_qblock_with_mushroom(level, { x, y }, MushroomState::Type::Red);
    else if (tile == Tile::QBlockGreenMushroom) put_qblock_with_mushroom(level, { x, y }, MushroomState::Type::Green);
    else if (tile == Tile::QBlockCoins) put_qblock_with_coins(level, { x, y });
    else if (tile == Tile::QBlockFireFlower) put_qblock_with_flower(level, { x, y });

    else if (tile == Tile::Hitbox) put_hitbox_block(level, { x, y });
    else if (tile == Tile::Finish) level.finish_position = { x, y };
    
    else if (tile == Tile::Checkpoint) put_checkpoint(level, { x, y });
    else if (&id_to_texture.at(tile).front() != level_generator::no_texture || tile == Tile::Dirt) {
      put_solid(level, { x, y }, id_to_texture.at(tile).front());
    }

    counter++;
  }
}

} //namespace level_generator
