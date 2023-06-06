#pragma once

#include "Renderer/Renderer.hpp"

#include "States/BlockState.hpp"
#include "States/BossState.hpp"
#include "States/LevelState.hpp"

#include "States/PlatformState.hpp"
#include "Util/Util.hpp"

#include "config.hpp"
#include "res/textures.hpp"

#include "PolyControllers.hpp"

static auto is_component_on_screen(const util::Rect& component, const glm::vec2& offset){
  using config::FrameBufferSize;

  const auto& position = component.position;
  const auto& size = component.size;

  const auto is_x = position.x - offset.x == util::in_range(-size.x, FrameBufferSize.x);
  const auto is_y = position.y - offset.y == util::in_range(-size.y, FrameBufferSize.y);

  return is_x && is_y;
}

static auto render_entity(const EntityState& entity, const glm::vec2& offset){
    if (!is_component_on_screen(entity, offset)) return;

    renderer::draw(Drawable{
      entity.position - offset,
      entity.size,
      entity.current_texture,
      1.f,
      { entity.direction * entity.texture_flip, entity.vertical_flip },
      entity.is_visible
    });
}

template<>
struct EntitiesView<EntityState>{
  static auto run(const EntityState& entity, const glm::vec2& offset){
    render_entity(entity, offset);
  }
};

template<> struct EntitiesView<GoombaState> : EntitiesView<EntityState>{};
template<> struct EntitiesView<KoopaState> : EntitiesView<EntityState>{};
template<> struct EntitiesView<JumpingKoopaState> : EntitiesView<EntityState>{};
template<> struct EntitiesView<FlyingKoopaState> : EntitiesView<EntityState>{};
template<> struct EntitiesView<MushroomState> : EntitiesView<EntityState>{};
template<> struct EntitiesView<BeetleState> : EntitiesView<EntityState>{};
template<> struct EntitiesView<SpikeState> : EntitiesView<EntityState>{};
template<> struct EntitiesView<FishState> : EntitiesView<EntityState>{};
template<> struct EntitiesView<SquidState> : EntitiesView<EntityState>{};
template<> struct PlantsView<PlantState> : EntitiesView<EntityState>{};

static auto render_points_particles(const std::vector<PointsParticlesState>& points, const glm::vec2& offset){
  for (const auto& point : points){
    renderer::print(point.text, offset);
  }
}

template<> 
struct BlocksView<BlockState>{
  static auto run(const BlockBase& block, const glm::vec2& offset){
    if (!is_component_on_screen(block, offset)) return;

    renderer::draw(Drawable{
      block.position - offset,
      glm::vec2(BlockBase::Size),
      block.texture,
      block.alpha,
      { Drawable::Flip::NoFlip, Drawable::Flip::NoFlip },
      block.is_visible
    });
  }
};

template<> 
struct BackgroundView<BackgroundHillState> : BlocksView<BlockState>{};

template<> 
struct BackgroundView<BackgroundBushState> : BlocksView<BlockState>{};

template<>
struct BackgroundView<CloudState>{
  static auto run(
      const CloudState& cloud, 
      const LevelState& level,
      const glm::vec2& offset
  ){
    auto& textures = *level.cloud_textures;

    const auto& top_left = textures[0];
    const auto& top_right = textures[1];
    const auto& top_center = textures[2];
    const auto& bottom_left = textures[3];
    const auto& bottom_right = textures[4];
    const auto& bottom_center = textures[5];

    const auto [position, size] = cloud;
    const auto x = position.x - level.cloud_offset;
    const auto y = position.y;

    BlocksView<BlockState>::run(BlockState({ x, y  }, &top_left), offset);
    BlocksView<BlockState>::run(BlockState({ x, (y + 1)  }, &bottom_left), offset);

    for (int i = 0; i < size; ++i){
      BlocksView<BlockState>::run(BlockState({ (x + i + 1), y  }, &top_center), offset);
      BlocksView<BlockState>::run(BlockState({ (x + i + 1), (y + 1)  }, &bottom_center), offset);
    }

    BlocksView<BlockState>::run(BlockState({ (x + size + 1), y  }, &top_right), offset);
    BlocksView<BlockState>::run(BlockState({ (x + size + 1), (y + 1)  }, &bottom_right), offset);
  }
};

template<>
struct BlocksView<CoinBlockState>{
  static auto run(const CoinBlockState& block, const glm::vec2& offset){
    BlocksView<BlockState>::run(block, offset);
  }
};

template<>
struct BlocksView<BricksBlockState>{
  static auto run(const BricksBlockState& block, const glm::vec2& offset){
    for (const auto& particle : block.particles){
      render_entity(particle, offset);
    }

    BlocksView<BlockState>::run(block, offset);
  }
};

static auto render_player(const PlayerState& player, const glm::vec2& offset){
  for (const auto& fireball : player.fireballs){
    render_entity(fireball, offset);
    BlocksView<BlockState>::run(fireball.explosion, offset);
  } 

  render_entity(player, offset);
}

template<>
struct EntitiesView<FireBarState>{
  static auto run(const FireBarState& bar, const glm::vec2& offset){
    for (const auto& fireball : bar.fireballs){
      render_entity(fireball, offset);
    }
  }
};

template<>
struct EntitiesView<HammerBroState>{
  static auto run(const HammerBroState& bro, const glm::vec2& offset){
    for (auto& item : bro.hammer_generator.items){
      render_entity(item, offset);
    }

    render_entity(EntityState(bro), offset);
  }
};

template<>
struct EntitiesView<PlatformState>{
  static auto run(const PlatformState& platform, const glm::vec2& offset){
    for (int i = 0; i < platform.width; ++i){
      renderer::draw(Drawable{
        platform.position + glm::vec2(i * PlatformState::ElementSize, 0) - offset,
        glm::vec2(PlatformState::ElementSize),
        &textures::platform
      });
    }
  }
};

template<> struct EntitiesView<LoopedPlatformState> : EntitiesView<PlatformState> {};

template<>
struct BlocksView<QBlockState<CoinPusherState>>{
  static auto run(const QBlockState<CoinPusherState>& block, const glm::vec2& offset){
    for (auto& coin : block.pusher.coins){
      BlocksView<BlockState>::run(coin, offset);
    }

    BlocksView<BlockState>::run(block, offset);
  }
};

template<>
struct BlocksView<QBlockState<FireFlowerPusherState>>{
  static auto run(const QBlockState<FireFlowerPusherState>& block, const glm::vec2& offset){
    BlocksView<BlockState>::run(block.pusher.fire_flower, offset);
    BlocksView<BlockState>::run(block, offset);
  }
};

template<typename Entity>
struct BlocksView<QBlockState<EntityPusherState<Entity>>>{
  static auto run(
      const QBlockState<EntityPusherState<Entity>>& block, 
      const glm::vec2& offset
  ){
    EntitiesView<Entity>::run(block.pusher.entity, offset);

    BlocksView<BlockState>::run(block, offset);
  }
};

template<>
struct EntitiesView<BossState>{
  static auto run(const BossState& boss, const glm::vec2& offset){
    renderer::highlight_mode = boss.is_highlighted;
    render_entity(boss, offset);
    renderer::highlight_mode = false;
  }
};

template<> struct EntitiesView<KingGoombaState> : EntitiesView<BossState>{};
template<> struct EntitiesView<KingKoopaState> : EntitiesView<BossState>{};

template<> struct EntitiesView<KingBeetleState>{
  static auto run(const KingBeetleState& boss, const glm::vec2& offset){
    for (const auto& f : boss.fireballs){
      render_entity(f, offset);
      BlocksView<BlockState>::run(f.explosion, offset);
    }

    EntitiesView<BossState>::run(boss, offset);
  }
};


