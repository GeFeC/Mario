#pragma once

#include "LevelGenerator/Tiles.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"
#include <unordered_set>
#include <unordered_map>

namespace mario::level_generator{

const auto no_texture = &textures::dirt;

using pair = std::pair<Tile, renderer::TextureGroup>;
static auto id_to_texture = std::unordered_map<Tile, renderer::TextureGroup>{
  pair(Tile::Air, no_texture),
  pair(Tile::Checkpoint, no_texture),
  pair(Tile::Hitbox, no_texture),
  pair(Tile::Finish, no_texture),
  pair(Tile::Dirt, &textures::dirt),
  pair(Tile::BlueDirt, &textures::blue_dirt),
  pair(Tile::GreenGround, &textures::green_ground),
  pair(Tile::WaterGround, &textures::water_ground),
  pair(Tile::CastleStone, &textures::castle_stone),
  pair(Tile::Coral, &textures::coral),
  pair(Tile::Bridge, &textures::bridge),
  pair(Tile::Cloud, &textures::cloud_block),
  pair(Tile::UnstableCloud, &textures::cloud_unstable),

  pair(Tile::CannonTop, texture_groups::cannon),
  pair(Tile::CannonBottom, &textures::cannon_bottom),

  pair(Tile::MushroomRight, &textures::mushroom_right),
  pair(Tile::MushroomLeft, &textures::mushroom_left),
  pair(Tile::MushroomCenter, &textures::mushroom_center),
  pair(Tile::MushroomBot2, &textures::mushroom_bot2),
  pair(Tile::MushroomBot1, &textures::mushroom_bot1),

  pair(Tile::GrassCenter, &textures::grass_center),
  pair(Tile::GrassLeft, &textures::grass_left),
  pair(Tile::GrassRight, &textures::grass_right),
  pair(Tile::GrassBot, &textures::grass_bot),

  pair(Tile::SnowCenter, &textures::snow_center),
  pair(Tile::SnowLeft, &textures::snow_left),
  pair(Tile::SnowRight, &textures::snow_right),
  pair(Tile::SnowBot, &textures::snow_bot),

  pair(Tile::Bricks, texture_groups::bricks),
  pair(Tile::BlueBricks, texture_groups::blue_bricks),
  pair(Tile::QBlock, no_texture),
  pair(Tile::QBlockMushroom, texture_groups::mushroom),
  pair(Tile::QBlockGreenMushroom, texture_groups::green_mushroom),
  pair(Tile::QBlockCoins, texture_groups::spinning_coin),
  pair(Tile::QBlockFireFlower, texture_groups::fire_flower),

  pair(Tile::RedPipeTopRight, &textures::red_pipe_top_right),
  pair(Tile::RedPipeBottomRight, &textures::red_pipe_bottom_right),
  pair(Tile::RedPipeTopLeft, &textures::red_pipe_top_left),
  pair(Tile::RedPipeBottomLeft, &textures::red_pipe_bottom_left),

  pair(Tile::GreenPipeTopRight, &textures::green_pipe_top_right),
  pair(Tile::GreenPipeBottomRight, &textures::green_pipe_bottom_right),
  pair(Tile::GreenPipeTopLeft, &textures::green_pipe_top_left),
  pair(Tile::GreenPipeBottomLeft, &textures::green_pipe_bottom_left),
  pair(Tile::GreenPipeBottom, &textures::green_pipe_bottom_horizontal),
  pair(Tile::GreenPipeTop, &textures::green_pipe_top_horizontal),
  pair(Tile::GreenPipeBottomOutput, &textures::green_pipe_bottom_horizontal_output),
  pair(Tile::GreenPipeTopOutput, &textures::green_pipe_top_horizontal_output),

  pair(Tile::WhitePipeTopRight, &textures::white_pipe_top_right),
  pair(Tile::WhitePipeBottomRight, &textures::white_pipe_bottom_right),
  pair(Tile::WhitePipeTopLeft, &textures::white_pipe_top_left),
  pair(Tile::WhitePipeBottomLeft, &textures::white_pipe_bottom_left),

  pair(Tile::GreenHillTop, &textures::green_hill_top),
  pair(Tile::GreenHillRight, &textures::green_hill_right),
  pair(Tile::GreenHillLeft, &textures::green_hill_left),
  pair(Tile::GreenHillCenter, &textures::green_hill_center),
  pair(Tile::GreenHillCenter2, &textures::green_hill_center_dot),

  pair(Tile::RedHillTop, &textures::red_hill_top),
  pair(Tile::RedHillRight, &textures::red_hill_right),
  pair(Tile::RedHillLeft, &textures::red_hill_left),
  pair(Tile::RedHillCenter, &textures::red_hill_center),
  pair(Tile::RedHillCenter2, &textures::red_hill_center_dot),

  pair(Tile::WhiteHillTop, &textures::white_hill_top),
  pair(Tile::WhiteHillRight, &textures::white_hill_right),
  pair(Tile::WhiteHillLeft, &textures::white_hill_left),
  pair(Tile::WhiteHillCenter, &textures::white_hill_center),
  pair(Tile::WhiteHillCenter2, &textures::white_hill_center_dot),

  pair(Tile::Fireball, no_texture),

  pair(Tile::SmallStone, &textures::small_stone),

  pair(Tile::RedBushRight, &textures::red_bush_right),
  pair(Tile::RedBushLeft, &textures::red_bush_left),
  pair(Tile::RedBushCenter, &textures::red_bush_center),

  pair(Tile::GreenBushRight, &textures::green_bush_right),
  pair(Tile::GreenBushLeft, &textures::green_bush_left),
  pair(Tile::GreenBushCenter, &textures::green_bush_center),

  pair(Tile::WhiteBushRight, &textures::white_bush_right),
  pair(Tile::WhiteBushLeft, &textures::white_bush_left),
  pair(Tile::WhiteBushCenter, &textures::white_bush_center),

  pair(Tile::Coin, texture_groups::coin),
  pair(Tile::PurpleCoin, texture_groups::purple_coin),

  pair(Tile::Stone, &textures::stone),
  pair(Tile::BlueStone, &textures::blue_stone),

  //Entities
  pair(Tile::YellowGoomba, texture_groups::yellow_goomba),
  pair(Tile::RedGoomba, texture_groups::red_goomba),
  pair(Tile::Goomba, texture_groups::goomba),
  pair(Tile::FlameGoomba, texture_groups::flame_goomba),
  pair(Tile::Spike, texture_groups::spike),
  pair(Tile::GreenPlant, texture_groups::plant),
  pair(Tile::RedPlant, texture_groups::red_plant),
  pair(Tile::BlackPlant, texture_groups::black_plant),
  pair(Tile::FlameKoopa, texture_groups::flame_koopa),
  pair(Tile::FlameKoopaWings, texture_groups::flame_flying_koopa),
  pair(Tile::RedKoopa, texture_groups::red_koopa),
  pair(Tile::RedKoopaWings, texture_groups::red_flying_koopa),
  pair(Tile::GreenKoopa, texture_groups::green_koopa),
  pair(Tile::GreenKoopaWings, texture_groups::green_flying_koopa),
  pair(Tile::PurpleKoopa, texture_groups::purple_koopa),
  pair(Tile::PurpleKoopaWings, texture_groups::purple_flying_koopa),
  pair(Tile::Hammerbro, texture_groups::hammerbro),
  pair(Tile::RedHammerbro, texture_groups::red_hammerbro),
  pair(Tile::Beetle, texture_groups::beetle),
  pair(Tile::GreyFish, texture_groups::grey_fish),
  pair(Tile::RedFish, texture_groups::red_fish),
  pair(Tile::Squid, texture_groups::squid),
  pair(Tile::Bowser, texture_groups::bowser),
};

static auto allocated_textures = std::unordered_set<Tile>{};

} //namespace mario::level_generator
