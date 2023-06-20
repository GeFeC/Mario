#pragma once

#include "Controllers/CollisionController.hpp"
#include "Renderer/Drawable.hpp"
#include "Renderer/Renderer.hpp"

#include "States/BlockState.hpp"
#include "States/BossState.hpp"
#include "States/LevelState.hpp"

#include "States/PlatformState.hpp"
#include "States/PointsParticlesState.hpp"
#include "Util/Util.hpp"

#include "config.hpp"
#include "res/textures.hpp"

#include "PolyControllers.hpp"

namespace mario::views{

static auto is_component_on_screen(const collision_controller::Rect& component, const glm::vec2& offset){
  using config::FrameBufferSize;

  const auto& position = component.position;
  const auto& size = component.size;

  const auto is_x = position.x - offset.x == util::in_range(-size.x, FrameBufferSize.x);
  const auto is_y = position.y - offset.y == util::in_range(-size.y, FrameBufferSize.y);

  return is_x && is_y;
}

static auto render_points_particles(const std::vector<PointsParticlesState>& points, const glm::vec2& offset){
  for (const auto& point : points){
    renderer::print(point.text, offset);
  }
}

} //namespace mario::views

namespace mario{

template<>
struct EntitiesView<EntityState>{
  static auto run(const EntityState& entity, const glm::vec2& offset){
    if (!views::is_component_on_screen(entity, offset)) return;

    renderer::draw(renderer::Drawable{
      entity.position - offset,
      entity.size,
      entity.current_texture,
      1.f,
      { entity.direction * entity.texture_flip, entity.vertical_flip },
      entity.is_visible
    });
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

template<> 
struct BlocksView<BlockState>{
  static auto run(const BlockBase& block, const glm::vec2& offset){
    if (!views::is_component_on_screen(block, offset)) return;

    using renderer::Drawable;
    renderer::draw(Drawable{
      block.position - offset,
      glm::vec2(BlockBase::Size),
      block.texture,
      block.alpha,
      { EntityState::Flip::NoFlip, EntityState::Flip::NoFlip },
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
      EntitiesView<EntityState>::run(particle, offset);
    }

    BlocksView<BlockState>::run(block, offset);
  }
};

static auto render_player(const PlayerState& player, const glm::vec2& offset){
  for (const auto& fireball : player.fireballs){
    EntitiesView<EntityState>::run(fireball, offset);
    BlocksView<BlockState>::run(fireball.explosion, offset);
  } 

  EntitiesView<EntityState>::run(player, offset);
}

template<>
struct EntitiesView<FireBarState>{
  static auto run(const FireBarState& bar, const glm::vec2& offset){
    for (const auto& fireball : bar.fireballs){
      EntitiesView<EntityState>::run(fireball, offset);
    }
  }
};

template<>
struct EntitiesView<HammerBroState>{
  static auto run(const HammerBroState& bro, const glm::vec2& offset){
    for (auto& item : bro.hammer_generator.items){
      EntitiesView<EntityState>::run(item, offset);
    }

    EntitiesView<EntityState>::run(bro, offset);
  }
};

template<>
struct EntitiesView<PlatformState>{
  static auto run(const PlatformState& platform, const glm::vec2& offset){
    for (int i = 0; i < platform.width; ++i){
      using renderer::Drawable;

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
    EntitiesView<EntityState>::run(boss, offset);
    renderer::highlight_mode = false;
  }
};

template<> struct EntitiesView<KingGoombaState> : EntitiesView<BossState>{};
template<> struct EntitiesView<KingKoopaState> : EntitiesView<BossState>{};

template<> struct EntitiesView<KingCheepState>{ 
  static auto run(const KingCheepState& boss, const glm::vec2& offset){
    auto drawable = renderer::RotatableDrawable();

    drawable.position = boss.position;
    drawable.size = boss.size;
    drawable.rotation = boss.rotation;
    drawable.texture = boss.current_texture;
    drawable.flip.vertical = boss.vertical_flip;

    renderer::highlight_mode = boss.is_highlighted;
    renderer::draw(drawable);
    renderer::highlight_mode = false;
  }
};

template<> struct EntitiesView<KingBeetleState>{
  static auto run(const KingBeetleState& boss, const glm::vec2& offset){
    for (const auto& f : boss.fireballs){
      EntitiesView<EntityState>::run(f, offset);
      BlocksView<BlockState>::run(f.explosion, offset);
    }

    EntitiesView<BossState>::run(boss, offset);
  }
};

} //namespace mario
