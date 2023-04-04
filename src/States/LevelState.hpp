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
#include "States/JumpingKoopaState.hpp"
#include "States/FlyingKoopaState.hpp"
#include "States/MushroomState.hpp"
#include "States/SpikeState.hpp"
#include "States/BeetleState.hpp"
#include "States/FireBarState.hpp"
#include "States/HammerBroState.hpp"
#include "States/BossState.hpp"

#include "Renderer/Text.hpp"
#include "config.hpp"
#include "res/fonts.hpp"
#include "res/textures.hpp"
#include "Util/Util.hpp"

#include <memory>
#include <vector>

struct StatsState{
  int hp = 5;
  int score = 0;
  int time = 100;
  int coins = 0;
  int level_major = 1;
  int level_minor = 1;

  int max_boss_hp = 0;
  int* boss_hp = nullptr;
};

struct LevelState{
  inline static constexpr auto VerticalLevelHeight = 150.f;
  inline static constexpr auto MaxLevelScrollY = (VerticalLevelHeight - config::BlocksInColumn) * config::BlockSize;
  inline static constexpr auto CheckpointNotSet = glm::vec2(-1);
  inline static constexpr auto MinPlayerRelativeY = 5 * config::BlockSize;
  inline static constexpr auto MaxPlayerRelativeY = 9 * config::BlockSize;

  inline static float timer = 0.f;
  inline static auto blink_state = 0;
  inline static auto coin_spin_counter = util::InfiniteCounter(4.f, 20.f);
  inline static auto fire_flower_blink_counter = util::InfiniteCounter(4.f, 15.f);

  enum class Type{
    Horizontal,
    Vertical,
    Boss
  } type = Type::Horizontal;

  util::vector2d<int> hitbox_grid;

  Texture const* background_texture = nullptr;
  StatsState stats;
  PlayerState player;

  struct Blocks{
    std::vector<BlockState> entity_hitbox_blocks;
    std::vector<BlockState> normal;
    std::vector<CoinBlockState> coins;
    std::vector<QBlockState> q_blocks;
    std::vector<BricksBlockState> bricks;
    std::vector<SpinningCoinState> spinning_coins;
    std::vector<FireFlowerState> fire_flowers;
  } blocks;

  std::vector<FireBarState> fire_bars;

  struct Entities{
    std::vector<GoombaState> goombas;
    std::vector<GoombaState> red_goombas;
    std::vector<GoombaState> yellow_goombas;
    std::vector<KoopaState> green_koopas;
    std::vector<KoopaState> purple_koopas;
    std::vector<JumpingKoopaState> green_jumping_koopas;
    std::vector<FlyingKoopaState> green_flying_koopas;
    std::vector<KoopaState> red_koopas;
    std::vector<JumpingKoopaState> red_jumping_koopas;
    std::vector<FlyingKoopaState> red_flying_koopas;
    std::vector<BeetleState> beetles;
    std::vector<SpikeState> spikes;
    std::vector<MushroomState> mushrooms;
    std::vector<MushroomState> green_mushrooms;
    std::vector<PlantState> plants;
    std::vector<PlantState> red_plants;
    std::vector<HammerBroState> hammerbros;
  } entities;

  struct Bosses{
    std::unique_ptr<KingGoombaState> king_goomba; 
  } bosses;

  struct Background{
    std::vector<BlockState> hills;
    std::vector<BlockState> bushes;
    std::vector<CloudState> clouds;
  } background;

  std::vector<glm::vec2> checkpoints;
  glm::vec2 current_checkpoint = CheckpointNotSet;

  util::InfiniteCounter fireball_counter;
  util::InfiniteCounter hammer_counter;
  util::InfiniteCounter purple_koopa_counter;

  float camera_offset_y = MaxLevelScrollY;

  float load_delay = 3.f;
  float finish_delay = 2.f;
  float score_adding_after_finish_delay = 0.f;
  bool is_finished = false;

  LevelState() : 
    fireball_counter(4.f, 20.f), 
    hammer_counter(4.f, 10.f),
    purple_koopa_counter(10.f, 10.f) {}

  auto get_size() const{
    if (type == Type::Horizontal){
      return glm::vec2(
        config::HorizontalLevelWidth,
        config::BlocksInColumn
      );
    }

    if (type == Type::Boss){
      return glm::vec2(
        config::BlocksInRow,
        config::BlocksInColumn
      );
    }

    return glm::vec2(
      config::VerticalLevelWidth,
      config::HorizontalLevelWidth
    );
  }

  auto generate_hitbox_grid(){
    hitbox_grid.resize(get_size().x, std::vector<int>(get_size().y, 0));
  }

  auto get_finishing_pipe_position() const{
    if (type == Type::Horizontal) return glm::vec2(197, 8);
    if (type == Type::Vertical) return glm::vec2(16, 55);
    return glm::vec2(-1, -1);
  }

  auto& get_hitbox_grid_element(const glm::vec2& position){
    return hitbox_grid[position.x][position.y];
  }
};
