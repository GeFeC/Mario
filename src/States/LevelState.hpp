#pragma once

#include "States/BlockState.hpp"
#include "States/EntityPusherState.hpp"
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
#include "States/PlatformState.hpp"

#include "States/CoinPusherState.hpp"
#include "States/FireFlowerPusherState.hpp"

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
  inline static constexpr auto BlocksInRow = 20;
  inline static constexpr auto BlocksInColumn = 12;

  inline static constexpr auto HorizontalLevelSize = glm::vec2(200, BlocksInColumn);
  inline static constexpr auto MaxVerticalLevelSize = glm::vec2(BlocksInRow, 150);
  inline static constexpr auto BossLevelSize = glm::vec2(BlocksInRow, BlocksInColumn);

  inline static constexpr auto CheckpointNotSet = glm::vec2(-1);
  inline static constexpr auto MinPlayerRelativeY = 5 * BlockBase::Size;
  inline static constexpr auto MaxPlayerRelativeY = 9 * BlockBase::Size;

  static constexpr auto PlayerPositionToStartLevelScrolling = glm::vec2(9 * BlockBase::Size, 0.f);
  static constexpr auto PlayerYToRestartLevel = 10000.f;

  inline static float timer = 0.f;

  enum class Type{
    Horizontal,
    Vertical,
    Boss
  } type = Type::Horizontal;

  util::vector2d<int> hitbox_grid;

  Texture const* background_texture = nullptr;
  TextureGroup const* cloud_textures = nullptr;

  StatsState stats;
  PlayerState player;

  struct Blocks{
    std::vector<BlockState> normal;
    std::vector<CoinBlockState> coins;
    std::vector<BricksBlockState> bricks;

    std::vector<QBlockState<CoinPusherState>> q_blocks_with_coins;
    std::vector<QBlockState<FireFlowerPusherState>> q_blocks_with_flower;
    std::vector<QBlockState<MushroomPusherState>> q_blocks_with_mushroom;
    std::vector<QBlockState<GoombaPusherState>> q_blocks_with_goomba;
    std::vector<QBlockState<JumpingKoopaPusherState>> q_blocks_with_jumping_koopa;

    template<typename Function>
    auto for_each_q_block(const Function& function){
      util::multi_for(
        function, 
        q_blocks_with_coins, q_blocks_with_flower,
        q_blocks_with_mushroom, q_blocks_with_goomba, q_blocks_with_jumping_koopa
      );
    }

    template<typename Function>
    auto for_each_q_block(const Function& function) const{
      const_cast<Blocks*>(this)->for_each_q_block([&](const auto& x){ function(x); });
    }
  } blocks;

  std::vector<PlatformState> platforms;
  std::vector<PlatformState> looped_platforms;
  std::vector<FireBarState> fire_bars;

  struct Entities{
    std::vector<GoombaState> goombas;
    std::vector<GoombaState> red_goombas;
    std::vector<GoombaState> yellow_goombas;
    std::vector<KoopaState> green_koopas;
    std::vector<KoopaState> red_koopas;
    std::vector<KoopaState> purple_koopas;
    std::vector<JumpingKoopaState> green_jumping_koopas;
    std::vector<JumpingKoopaState> red_jumping_koopas;
    std::vector<JumpingKoopaState> purple_jumping_koopas;
    std::vector<FlyingKoopaState> green_flying_koopas;
    std::vector<FlyingKoopaState> red_flying_koopas;
    std::vector<FlyingKoopaState> purple_flying_koopas;
    std::vector<BeetleState> beetles;
    std::vector<SpikeState> spikes;
    std::vector<MushroomState> mushrooms;
    std::vector<MushroomState> green_mushrooms;
    std::vector<PlantState> plants;
    std::vector<PlantState> red_plants;
    std::vector<HammerBroState> hammerbros;

    template<typename Function>
    auto for_each(const Function& function){
      util::multi_for(
        function,
        goombas, red_goombas, yellow_goombas,
        green_koopas, red_koopas, purple_koopas,
        green_jumping_koopas, red_jumping_koopas, purple_jumping_koopas,
        green_flying_koopas, red_flying_koopas, purple_flying_koopas,
        beetles, spikes,
        plants, red_plants,
        hammerbros
      );
    }

    template<typename Function>
    auto for_each(const Function& function) const{
      const_cast<Entities*>(this)->for_each([&](const auto& x){ function(x); });
    }
  } entities;

  struct Bosses{
    std::weak_ptr<BossState> current_boss;

    std::shared_ptr<KingGoombaState> king_goomba;
    std::shared_ptr<KingKoopaState> king_koopa;
  } bosses;

  struct Background{
    std::vector<BlockState> hills;
    std::vector<BlockState> bushes;
    std::vector<CloudState> clouds;
  } background;

  std::vector<glm::vec2> checkpoints;
  glm::vec2 current_checkpoint = CheckpointNotSet;
  glm::vec2 finish_position;

  glm::vec2 camera_offset;

  int blink_state = 0;
  float purple_flying_koopa_timer = 0.f;
  util::InfiniteCounter coin_spin_counter = util::InfiniteCounter(4.f, 20.f);
  util::InfiniteCounter fire_flower_blink_counter = util::InfiniteCounter(4.f, 15.f);
  util::InfiniteCounter fireball_counter = util::InfiniteCounter(4.f, 20.f);
  util::InfiniteCounter hammer_counter = util::InfiniteCounter(4.f, 10.f);
  util::InfiniteCounter purple_koopa_counter = util::InfiniteCounter(10.f, 10.f);

  float load_delay = 3.f;
  float finish_delay = 2.f;
  float score_adding_after_finish_delay = 0.f;
  bool is_finished = false;

  auto max_size() const{
    switch(type){
      default: return HorizontalLevelSize;
      case Type::Vertical: return MaxVerticalLevelSize;
      case Type::Boss: return BossLevelSize;
    }
  }

  auto generate_hitbox_grid(){
    const auto size = max_size();
    hitbox_grid.resize(size.x, std::vector<int>(size.y, 0));
  }

  auto& hitbox_grid_element(const glm::vec2& position){
    return hitbox_grid[position.x][position.y];
  }
};
