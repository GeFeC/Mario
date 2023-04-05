#pragma once

#include "LevelGenerator/Tiles.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"
#include <unordered_set>
#include <unordered_map>

namespace level_generator{

const auto no_texture = &textures::dirt;

using pair = std::pair<Tile, TextureGroup>;
static auto id_to_texture = std::unordered_map<Tile, TextureGroup>{
  pair(Tile::Air, no_texture),
  pair(Tile::Checkpoint, no_texture),
  pair(Tile::Hitbox, no_texture),
  pair(Tile::Dirt, &textures::dirt),

  pair(Tile::MushroomRight, &textures::mushroom_right),
  pair(Tile::MushroomLeft, &textures::mushroom_left), 
  pair(Tile::MushroomCenter, &textures::mushroom_center),
  pair(Tile::MushroomBot2, &textures::mushroom_bot2),
  pair(Tile::MushroomBot1, &textures::mushroom_bot1),

  pair(Tile::Bricks, texture_groups::bricks),
  pair(Tile::QBlock, no_texture),
  pair(Tile::QBlockMushroom, &textures::mushroom),
  pair(Tile::QBlockGreenMushroom, &textures::green_mushroom),
  pair(Tile::QBlockCoins, texture_groups::spinning_coin),
  pair(Tile::QBlockFireFlower, texture_groups::fire_flower),

  pair(Tile::RedPipeTopRight, &textures::red_pipe_top_right),
  pair(Tile::RedPipeBottomRight, &textures::red_pipe_bottom_right),
  pair(Tile::RedPipeTopLeft, &textures::red_pipe_top_left),
  pair(Tile::RedPipeBottomLeft, &textures::red_pipe_bottom_left),

  pair(Tile::GreenHillTop, &textures::green_hill_top),
  pair(Tile::GreenHillRight, &textures::green_hill_right),
  pair(Tile::RedHillTop, &textures::red_hill_top),
  pair(Tile::RedHillRight, &textures::red_hill_right),
  pair(Tile::RedHillLeft, &textures::red_hill_left),
  pair(Tile::RedHillCenter, &textures::red_hill_center),
  pair(Tile::RedHillCenter2, &textures::red_hill_center_dot),
  pair(Tile::GreenHillLeft, &textures::green_hill_left),
  pair(Tile::GreenHillCenter, &textures::green_hill_center),
  pair(Tile::GreenHillCenter2, &textures::green_hill_center_dot),

  pair(Tile::Fireball, no_texture),

  pair(Tile::YellowGoomba, texture_groups::yellow_goomba),
  pair(Tile::RedGoomba, texture_groups::red_goomba),
  pair(Tile::Goomba, texture_groups::goomba),
  pair(Tile::Spike, texture_groups::spike),
  pair(Tile::GreenPlant, texture_groups::plant),
  pair(Tile::RedPlant, texture_groups::red_plant),
  pair(Tile::RedKoopa, texture_groups::red_koopa),
  pair(Tile::RedKoopaWings, texture_groups::red_flying_koopa),
  pair(Tile::GreenKoopa, texture_groups::green_koopa),
  pair(Tile::GreenKoopaWings, texture_groups::green_flying_koopa),
  pair(Tile::Hammerbro, texture_groups::hammerbro),
  pair(Tile::Beetle, texture_groups::beetle),

  pair(Tile::GreenBushRight, &textures::green_bush_right),
  pair(Tile::RedBushRight, &textures::red_bush_right),
  pair(Tile::RedBushLeft, &textures::red_bush_left),
  pair(Tile::RedBushCenter, &textures::red_bush_center),
  pair(Tile::GreenBushLeft, &textures::green_bush_left),
  pair(Tile::GreenBushCenter, &textures::green_bush_center),
  pair(Tile::Coin, texture_groups::coin),

  pair(Tile::Stone, &textures::stone),
  pair(Tile::PurpleKoopa, texture_groups::purple_koopa)
};

static auto allocate_texture_if_needed(Tile tile){
  id_to_texture.at(tile).allocate();
}

static auto allocated_textures = std::unordered_set<Tile>{};

}
