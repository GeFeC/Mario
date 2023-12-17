#pragma once
#include <algorithm>
#include <iterator>

namespace mario::level_generator{

enum class Tile{
  Air = -1,
  Dirt = 0,
  CastleStone = 1,
  MushroomRight = 5,
  MushroomLeft = 6,
  MushroomCenter = 7,
  MushroomBot2 = 8,
  MushroomBot1 = 9,
  GrassBot = 10,
  GreenGround = 13,
  Cloud = 15,
  Bricks = 16,
  BlueStone = 17,
  BlueDirt = 18,
  BlueBricks = 19,
  QBlock = 20,
  WhitePipeTopRight = 21,
  WhitePipeBottomRight = 23,
  WhitePipeTopLeft = 25,
  WhitePipeBottomLeft = 27,
  RedPipeTopRight = 31,
  RedPipeBottomRight = 32,
  RedPipeTopLeft = 33,
  RedPipeBottomLeft = 34,
  GreenPipeTopRight = 35,
  GreenPipeBottomRight = 37,
  GreenPipeTopLeft = 39,
  GreenPipeTopOutput = 40,
  GreenPipeBottomLeft = 41,
  GreenPipeBottomOutput = 42,
  GreenPipeTop = 43,
  GreenPipeBottom = 44,
  GreenHillTop = 45,
  GreenHillRight = 46,
  RedHillTop = 47,
  RedHillLeft = 49,
  RedHillRight = 48,
  RedHillCenter = 50,
  RedHillCenter2 = 51,
  GreenHillLeft = 52,
  GreenHillCenter = 53,
  GreenHillCenter2 = 54,
  Fireball = 55,
  YellowGoomba = 56,
  RedGoomba = 57,
  Goomba = 58,
  Spike = 59,
  GreenPlant = 60,
  RedPlant = 61,
  RedKoopa = 62,
  RedKoopaWings = 63,
  GreenKoopa = 64,
  GreenKoopaWings = 65,
  Hammerbro = 66,
  Beetle = 67,
  GreenBushRight = 68,
  RedBushRight = 69,
  RedBushLeft = 70,
  RedBushCenter = 71,
  GreenBushLeft = 72,
  GreenBushCenter = 73,
  Coin = 74,
  QBlockMushroom = 75,
  QBlockGreenMushroom = 76,
  QBlockCoins = 77,
  QBlockFireFlower = 78,
  Hitbox = 79,
  Checkpoint = 80,
  Stone = 81,
  PurpleKoopa = 82,
  GrassCenter = 83,
  GrassLeft = 84,
  GrassRight = 85,
  PurpleKoopaWings = 86,
  Finish = 87,
  SnowBot = 88,
  SnowCenter = 89,
  SnowLeft = 90,
  SnowRight = 91,
  WhiteHillCenter2 = 92,
  WhiteHillCenter = 93,
  WhiteHillLeft = 94,
  WhiteHillRight = 95,
  WhiteHillTop = 96,
  WhiteBushCenter = 97,
  WhiteBushLeft = 98,
  WhiteBushRight = 99,
  RedHammerbro = 100,
  GreyFish = 101,
  RedFish = 102,
  Coral = 106,
  WaterGround = 107,
  Squid = 108,
  Bridge = 109,
  BlackPlant = 111,
  SmallStone = 116,
  UnstableCloud = 117,
  PurpleCoin = 187,
  FlameGoomba = 120,
	FlameKoopa = 188,
};

static constexpr Tile HillTiles[] = {
  Tile::GreenHillCenter, Tile::GreenHillCenter2, Tile::GreenHillTop, Tile::GreenHillLeft, Tile::GreenHillRight,
  Tile::RedHillCenter, Tile::RedHillCenter2, Tile::RedHillTop, Tile::RedHillLeft, Tile::RedHillRight,
  Tile::WhiteHillCenter, Tile::WhiteHillCenter2, Tile::WhiteHillTop, Tile::WhiteHillLeft, Tile::WhiteHillRight
};

static constexpr Tile BushTiles[] = {
  Tile::GreenBushCenter, Tile::GreenBushLeft, Tile::GreenBushRight,
  Tile::RedBushCenter, Tile::RedBushLeft, Tile::RedBushRight,
  Tile::WhiteBushCenter, Tile::WhiteBushLeft, Tile::WhiteBushRight
};

template<std::size_t N>
using TileArray = Tile[N];

template<std::size_t N>
static auto any_tile(const TileArray<N>& tiles, Tile target){
  return std::any_of(std::begin(tiles), std::end(tiles), [&](auto e){ return e == target; });
}

} //namespace mario::level_generator
