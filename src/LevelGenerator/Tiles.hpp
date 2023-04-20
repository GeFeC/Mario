#pragma once
#include <algorithm>
#include <iterator>

namespace level_generator{

enum class Tile{
  Air = -1,
  Hitbox = 79,
  Checkpoint = 80,
  Finish = 87,

  Dirt = 0,
  MushroomRight = 5,
  MushroomCenter = 7,
  MushroomLeft = 6,
  MushroomBot1 = 9,
  MushroomBot2 = 8,
  GrassCenter = 83,
  GrassLeft = 84,
  GrassRight = 85,
  GrassBot = 10,
  Cloud = 15,
  Bricks = 16,
  Coin = 74,
  QBlock = 20,
  QBlockMushroom = 75,
  QBlockGreenMushroom = 76,
  QBlockCoins = 77,
  QBlockFireFlower = 78,

  GreenPipeTopRight = 35,
  GreenPipeBottomRight = 37,
  GreenPipeTopLeft = 39,
  GreenPipeBottomLeft = 41,

  RedPipeTopRight = 31,
  RedPipeBottomRight = 32,
  RedPipeTopLeft = 33,
  RedPipeBottomLeft = 34,

  GreenHillTop = 45,
  GreenHillLeft = 52,
  GreenHillRight = 46,
  GreenHillCenter = 53,
  GreenHillCenter2 = 54,

  RedHillTop = 47,
  RedHillLeft = 49,
  RedHillRight = 48,
  RedHillCenter = 50,
  RedHillCenter2 = 51,

  Fireball = 55,

  Goomba = 58,
  RedGoomba = 57,
  YellowGoomba = 56,

  GreenPlant = 60,
  RedPlant = 61,

  RedKoopa = 62,
  RedKoopaWings = 63,
  GreenKoopa = 64,
  GreenKoopaWings = 65,
  PurpleKoopa = 82,
  PurpleKoopaWings = 86,

  Hammerbro = 66,
  Beetle = 67,
  Spike = 59,

  GreenBushRight = 68,
  GreenBushLeft = 72,
  GreenBushCenter = 73,

  RedBushRight = 69,
  RedBushLeft = 70,
  RedBushCenter = 71,

  Stone = 81
};

static constexpr Tile HillTiles[] = {
  Tile::GreenHillCenter, Tile::GreenHillCenter2, Tile::GreenHillTop, Tile::GreenHillLeft, Tile::GreenHillRight,
  Tile::RedHillCenter, Tile::RedHillCenter2, Tile::RedHillTop, Tile::RedHillLeft, Tile::RedHillRight
};

static constexpr Tile BushTiles[] = {
  Tile::GreenBushCenter, Tile::GreenBushLeft, Tile::GreenBushRight,
  Tile::RedBushCenter, Tile::RedBushLeft, Tile::RedBushRight
};

template<std::size_t N>
using TileArray = Tile[N];

template<std::size_t N>
static auto any_tile(const TileArray<N>& tiles, Tile target){
  return std::any_of(std::begin(tiles), std::end(tiles), [&](auto e){ return e == target; });
}

} //namespace level_generator