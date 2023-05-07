#pragma once

#include "Renderer/Renderer.hpp"

#include "States/EntityState.hpp"
#include "States/PlayerState.hpp"
#include "States/BlockState.hpp"
#include "States/PointsParticlesState.hpp"
#include "States/FireBarState.hpp"
#include "States/HammerBroState.hpp"
#include "States/BossState.hpp"
#include "States/PlatformState.hpp"
#include "States/QBlockState.hpp"

#include "States/CoinPusherState.hpp"
#include "States/FireFlowerPusherState.hpp"

#include "Util/Util.hpp"

#include "config.hpp"
#include "res/textures.hpp"

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
    { entity.direction * entity.texture_flip, entity.vertical_flip },
    entity.is_visible
  });
}

static auto render_boss(const BossState& boss, const glm::vec2& offset){
  renderer::highlight_mode = boss.is_highlighted;
  render_entity(boss, offset);
  renderer::highlight_mode = false;
}

static auto render_points_particles(const std::vector<PointsParticlesState>& points, const glm::vec2& offset){
  for (const auto& point : points){
    renderer::print(point.text, offset);
  }
}

static auto render_block(const BlockBase& block, const glm::vec2& offset){
  if (!is_component_on_screen(block, offset)) return;

  renderer::draw(Drawable{
    block.position - offset,
    glm::vec2(BlockBase::Size),
    block.texture,
    { Drawable::Flip::NoFlip, Drawable::Flip::NoFlip },
    block.is_visible
  });
}

static auto render_bricks(const BricksBlockState& block, const glm::vec2& offset){
  for (const auto& particle : block.particles){
    render_entity(particle, offset);
  }

  render_block(block, offset);
}

static auto render_player(const PlayerState& player, const glm::vec2& offset){
  for (const auto& fireball : player.fireballs){
    render_entity(fireball, offset);
    render_block(fireball.explosion, offset);
  } 

  render_entity(player, offset);
}

static auto render_fire_bar(const FireBarState& bar, const glm::vec2& offset){
  for (const auto& fireball : bar.fireballs){
    render_entity(fireball, offset);
  }
}

static auto render_entity(const HammerBroState& bro, const glm::vec2& offset){
  for (auto& item : bro.hammer_generator.items){
    render_entity(item, offset);
  }

  render_entity(EntityState(bro), offset);
}

static auto render_platform(const PlatformState& platform, const glm::vec2& offset){
  for (int i = 0; i < platform.width; ++i){
    renderer::draw(Drawable{
      platform.position + glm::vec2(i * PlatformState::ElementSize, 0) - offset,
      glm::vec2(PlatformState::ElementSize),
      &textures::platform
    });
  }
}

static auto render_q_block_with_coins(const QBlockState<CoinPusherState>& block, const glm::vec2& offset){
  for (auto& coin : block.pusher.coins){
    render_block(coin, offset);
  }

  render_block(block, offset);
}

template<typename Entity>
static auto render_q_block_with_entity(
    const QBlockState<EntityPusherState<Entity>>& block, 
    const glm::vec2& offset
){
  render_entity(block.pusher.entity, offset);

  render_block(block, offset);
}

static auto render_q_block_with_flower(const QBlockState<FireFlowerPusherState>& block, const glm::vec2& offset){
  render_block(block.pusher.fire_flower, offset);
  render_block(block, offset);
}
