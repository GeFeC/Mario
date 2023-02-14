#pragma once

#include "States/BlockState.hpp"
#include "States/EntityState.hpp"
#include "States/MonsterState.hpp"
#include "States/PlayerState.hpp"
#include "States/PointsParticlesState.hpp"
#include "States/LoopedCounter.hpp"
#include "States/QBlockState.hpp"
#include "States/FireFlowerState.hpp"
#include "States/PlantState.hpp"
#include "States/GoombaState.hpp"
#include "States/KoopaState.hpp"
#include "States/MushroomState.hpp"
#include "States/SpikeState.hpp"
#include "States/BeetleState.hpp"

#include "Renderer/Text.hpp"
#include "config.hpp"
#include "res/fonts.hpp"
#include "res/textures.hpp"
#include "Util.hpp"

#include <vector>

struct StatsState{
  int hp = 3;
  int score = 0;
  int time = 400;
  int coins = 0;
  int level_major = 1;
  int level_minor = 1;
};

struct LevelState{
  util::vector2d<int> hitbox_grid;

  StatsState stats;
  PlayerState player;
  inline static auto blink_state = 0;
  inline static auto coin_spin_counter = InfiniteCounter(4.f, 20.f);
  inline static auto fire_flower_blink_counter = InfiniteCounter(4.f, 15.f);

  struct Blocks{
    std::vector<BlockState> entity_hitbox_blocks;
    std::vector<BlockState> normal;
    std::vector<CoinBlockState> coins;
    std::vector<QBlockState> q_blocks;
    std::vector<BricksBlockState> bricks;
    std::vector<SpinningCoinState> spinning_coins;
    std::vector<FireFlowerState> fire_flowers;
  } blocks;

  struct Entities{
    std::vector<GoombaState> goombas;
    std::vector<GoombaState> red_goombas;
    std::vector<GoombaState> yellow_goombas;
    std::vector<KoopaState> green_koopas;
    std::vector<FlyingKoopaState> green_flying_koopas;
    std::vector<KoopaState> red_koopas;
    std::vector<FlyingKoopaState> red_flying_koopas;
    std::vector<BeetleState> beetles;
    std::vector<SpikeState> spikes;
    std::vector<MushroomState> mushrooms;
    std::vector<MushroomState> green_mushrooms;

    std::vector<PlantState> plants;
  } entities;

  struct Background{
    std::vector<BackgroundObjectState> hills;
    std::vector<BackgroundObjectState> bushes;
  } background;

  InfiniteCounter fireball_counter;

  float load_delay = 3.f;
  bool should_screen_scroll = false;

  LevelState() : fireball_counter(4.f, 20.f) {
    hitbox_grid.resize(config::MaxLevelSize, std::vector<int>(config::BlocksInColumn, 0));
  }

  auto& get_hitbox_grid_element(const glm::vec2& position){
    return hitbox_grid[position.x][position.y];
  }
};
