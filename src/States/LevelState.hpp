#pragma once

#include "States/FlameFlyingKoopaState.hpp"
#include "States/FlameJumpingKoopaState.hpp"
#include "States/UpfireState.hpp"
#include "States/FlameKoopaState.hpp"
#include "States/FlameGoombaState.hpp"
#include "States/StatsState.hpp"
#include "States/BlockState.hpp"
#include "States/EntityPusherState.hpp"
#include "States/PlayerState.hpp"
#include "States/QBlockState.hpp"
#include "States/PlantState.hpp"
#include "States/BlackPlantState.hpp"
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
#include "States/FishState.hpp"
#include "States/SquidState.hpp"
#include "States/LakitoState.hpp"
#include "States/BowserState.hpp"
#include "States/CannonState.hpp"

#include "States/CoinPusherState.hpp"
#include "States/FireFlowerPusherState.hpp"

#include "Util/LoopedCounter.hpp"

#include "res/textures.hpp"
#include "Util/Util.hpp"
#include "Util/Poly.hpp"
#include "LavaState.hpp"

#include <vector>
#include <limits>

namespace mario{

struct LevelState{
  using GameObjectsArray = util::poly::Array<
    //Background
    CloudState,
    BackgroundHillState,
    BackgroundBushState,
    LavaState,

    //Blocks
    BlockState, 
    CoinBlockState, PurpleCoinBlockState,
    BricksBlockState, 
    UnstableCloudState,
		CannonState,
    QBlockState<CoinPusherState>,
    QBlockState<FireFlowerPusherState>,
    QBlockState<MushroomPusherState>,
    QBlockState<GoombaPusherState>,
    QBlockState<KoopaPusherState>,
    QBlockState<JumpingKoopaPusherState>,
    QBlockState<FlyingKoopaPusherState>,
    QBlockState<HammerBroPusherState>,

    //Entities
    GoombaState,
    FlameGoombaState,
    KoopaState,
		FlameKoopaState,
    JumpingKoopaState,
    FlameJumpingKoopaState,
    FlyingKoopaState,
    FlameFlyingKoopaState,
    BeetleState,
    SpikeState,
    MushroomState,
    PlantState,
    BlackPlantState,
    HammerBroState,
    FishState,
    SquidState,
    LakitoState,
		BowserState,

    //Obstacles
		UpfireState,
    FireBarState,
    PlatformState,
    LoopedPlatformState,

    //Bosses
    KingGoombaState,
    KingKoopaState,
    KingBeetleState,
    KingCheepState,
    KingPlantState,
    KingLakitoState,
		KingBowserState	
  >;

  static constexpr auto BlocksInRow = 20;
  static constexpr auto BlocksInColumn = 12;

  static constexpr auto HorizontalLevelSize = glm::vec2(200, BlocksInColumn);
  static constexpr auto MaxVerticalLevelSize = glm::vec2(BlocksInRow, 150);
  static constexpr auto BossLevelSize = glm::vec2(BlocksInRow, BlocksInColumn);

  static constexpr auto CheckpointNotSet = glm::vec2(-1);
  static constexpr auto MinPlayerRelativeY = 5 * BlockBase::Size;
  static constexpr auto MaxPlayerRelativeY = 9 * BlockBase::Size;

  static constexpr auto PlayerPositionToStartLevelScrolling = glm::vec2(9 * BlockBase::Size, 0.f);
  static constexpr auto PlayerYToRestartLevel = 10000.f;

  inline static float timer = 0.f;

  enum class Type{
    Horizontal,
    Vertical,
    Boss
  } type = Type::Horizontal;

  enum class HitboxState{
    NonSolid,
    Solid
  };

  util::vector2d<HitboxState> hitbox_grid;

  renderer::Texture const* background_texture = nullptr;
  renderer::TextureGroup const* cloud_textures = nullptr;

  GameObjectsArray game_objects;
  StatsState stats;
  PlayerState player;

  std::vector<glm::vec2> checkpoints;
  glm::vec2 current_checkpoint = CheckpointNotSet;
  glm::vec2 finish_position;

  glm::vec2 camera_offset;

  float background_opacity = 0.f;
  util::InfiniteCounter background_pulse_counter = util::InfiniteCounter(5.f, 1.f);
  util::InfiniteCounter coin_spin_counter = util::InfiniteCounter(4.f, 20.f);
  util::InfiniteCounter fire_flower_blink_counter = util::InfiniteCounter(4.f, 15.f);
  util::InfiniteCounter fireball_counter = util::InfiniteCounter(4.f, 20.f);
  util::InfiniteCounter hammer_counter = util::InfiniteCounter(4.f, 10.f);
  util::InfiniteCounter purple_koopa_counter = util::InfiniteCounter(10.f, 10.f);

  float purple_flying_koopa_timer = 0.f;

  float cloud_offset = 0.f;
  float water_level = util::BigValue;
	float lava_offset = 0.f;

  float load_delay = 3.f;
  float finish_delay = 2.f;
  float score_adding_after_finish_delay = 0.f;

  int blink_state = 0;

  bool is_finished = false;
  bool is_dark = false;
  bool should_handle_hitbox_on_sides = true;

  int min_scroll_y = std::numeric_limits<int>::max();

  auto max_size() const{
    switch(type){
      default: return HorizontalLevelSize;
      case Type::Vertical: return MaxVerticalLevelSize;
      case Type::Boss: return BossLevelSize;
    }
  }

  auto initialise_hitbox_grid(){
    const auto size = max_size();
    hitbox_grid.resize(size.x, std::vector<HitboxState>(size.y, HitboxState::NonSolid));
  }

  auto& hitbox_grid_element(const glm::vec2& position){
    return hitbox_grid[position.x][position.y];
  }

  auto is_level_underground() const{
    return background_texture == &textures::underground_bg;
  }

  auto is_level_in_castle() const{
    return background_texture == &textures::castle_bg;
  }
};

} //namespace mario
