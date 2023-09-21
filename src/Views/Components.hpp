#pragma once

#include "Controllers/CollisionController.hpp"
#include "Renderer/Drawable.hpp"
#include "Renderer/Renderer.hpp"

#include "States/BlackPlantState.hpp"
#include "States/BlockState.hpp"
#include "States/BossState.hpp"
#include "States/LakitoState.hpp"
#include "States/LevelState.hpp"

#include "States/PlatformState.hpp"
#include "States/PointsParticlesState.hpp"
#include "Util/Interval.hpp"

#include "config.hpp"
#include "res/textures.hpp"

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

template<typename T, typename = util::not_derived<T, EntityState>>
static auto render_entity(const T&, const LevelState&) {}

static auto render_entity(const PlantState&, const LevelState&) {}
static auto render_entity(const BlackPlantState&, const LevelState&) {}

template<typename T, typename = util::not_derived<T, BlockBase>>
static auto render_block(const T&, const LevelState&){}

template<typename T>
static auto render_background(const T&, const LevelState&) {}

template<typename T>
static auto render_plant(const T&, const LevelState&) {}

static auto render_entity(const EntityState& entity, const LevelState& level){
  const auto& offset = level.camera_offset;
  if (!views::is_component_on_screen(entity, offset)) return;

  auto drawable = renderer::Drawable{};
  drawable.position = entity.position - offset;
  drawable.size = entity.size;
  drawable.texture = entity.current_texture;
  drawable.is_visible = entity.is_visible;
  drawable.flip = { 
    util::Flip(entity.direction.as_int() * entity.texture_flip.as_int()), 
    entity.vertical_flip 
  };

  renderer::draw(drawable);
}

static auto render_entity(const LakitoState& lakito, const LevelState& level){
  render_entity(lakito.thrown_spike, level);
  render_entity(lakito | util::as<EntityState>, level);
}

static auto render_block(const BlockBase& block, const LevelState& level){
  const auto& offset = level.camera_offset;
  if (!views::is_component_on_screen(block, offset)) return;

  auto drawable = renderer::Drawable{};
  drawable.position = block.position - offset;
  drawable.size = glm::vec2(BlockBase::Size);
  drawable.texture = block.texture;
  drawable.alpha = block.alpha;
  drawable.flip = { util::Flip::no_flip(), util::Flip::no_flip() };
  drawable.is_visible = block.is_visible;

  renderer::draw(drawable);
}

static auto render_background(const CloudState& cloud, const LevelState& level){
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

  render_block(BlockState({ x, y  }, &top_left), level);
  render_block(BlockState({ x, y + 1  }, &bottom_left), level);

  for (int i = 0; i < size; ++i){
    render_block(BlockState({ x + i + 1, y  }, &top_center), level);
    render_block(BlockState({ x + i + 1, y + 1  }, &bottom_center), level);
  }

  render_block(BlockState({ x + size + 1, y  }, &top_right), level);
  render_block(BlockState({ x + size + 1, y + 1  }, &bottom_right), level);
}

static auto render_block(const BricksBlockState& block, const LevelState& level){
  for (const auto& particle : block.particles){
    render_entity(particle, level);
  }

  render_block(block | util::as<BlockBase>, level);
}

static auto render_player(const PlayerState& player, const LevelState& level){
  for (const auto& fireball : player.fireballs){
    render_entity(fireball, level);
    render_block(fireball.explosion, level);
  } 

  render_entity(player, level);
}

static auto render_entity(const FireBarState& bar, const LevelState& level){
  for (const auto& fireball : bar.fireballs){
    render_entity(fireball, level);
  }
}

static auto render_entity(const HammerBroState& bro, const LevelState& level){
  for (auto& item : bro.hammer_generator.items){
    render_entity(item, level);
  }

  render_entity(bro | util::as<EntityState>, level);
}

static auto render_entity(const PlatformState& platform, const LevelState& level){
  for (int i = 0; i < platform.width; ++i){
    auto drawable = renderer::Drawable{};

    drawable.position = platform.position + glm::vec2(i * PlatformState::ElementSize, 0) - level.camera_offset;
    drawable.size = glm::vec2(PlatformState::ElementSize);
    drawable.texture = &textures::platform;

    renderer::draw(drawable);
  }
}

static auto render_entity(const LoopedPlatformState& platform, const LevelState& level){
  render_entity(platform | util::as<PlatformState>, level);
}

static auto render_block(const QBlockState<CoinPusherState>& block, const LevelState& level){
  for (auto& coin : block.pusher.coins){
    render_block(coin, level);
  }

  render_block(block | util::as<BlockBase>, level);
}

static auto render_block(const QBlockState<FireFlowerPusherState>& block, const LevelState& level){
  render_block(block.pusher.fire_flower, level);
  render_block(block | util::as<BlockBase>, level);
}

template<typename T>
static auto render_block(const QBlockState<EntityPusherState<T>>& block, const LevelState& level){
  render_entity(block.pusher.entity, level);

  render_block(block | util::as<BlockBase>, level);
}

static auto render_entity(const BossState& boss, const LevelState& level){
  renderer::highlight_mode = boss.is_highlighted;
  render_entity(boss | util::as<EntityState>, level);
  renderer::highlight_mode = false;
}

static auto render_entity(const KingCheepState& boss, const LevelState& level){
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

static auto render_entity(const KingBeetleState& boss, const LevelState& level){
  for (const auto& f : boss.fireballs){
    render_entity(f, level);
    render_block(f.explosion, level);
  }

  render_entity(boss | util::as<BossState>, level);
}

static auto render_plant(const BlackPlantState& plant, const LevelState& level){
  for (const auto& f : plant.fireball_generator.items){
    render_entity(f, level);
    render_block(f.explosion, level);
  }

  render_entity(plant | util::as<EntityState>, level);
}

static auto render_plant(const KingPlantState& boss, const LevelState& level){
  renderer::highlight_mode = boss.is_highlighted;
  render_plant(boss | util::as<BlackPlantState>, level);
  renderer::highlight_mode = false;
}

static auto render_entity(const KingPlantState& boss, const LevelState& level){}

static auto render_plant(const PlantState& plant, const LevelState& level){
  render_entity(plant | util::as<EntityState>, level);
}

static auto render_background(const BackgroundBushState& bush, const LevelState& level){
  render_block(bush, level);
}

static auto render_background(const BackgroundHillState& bush, const LevelState& level){
  render_block(bush, level);
}

} //namespace mario::views
