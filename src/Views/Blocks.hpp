#pragma once

#include "Views/Util.hpp"
#include "Views/Entities.hpp"
#include "States/BlockState.hpp"
#include "States/LevelState.hpp"

namespace mario::views{

template<typename T, typename = util::not_derived<T, BlockBase>>
static auto render_block(const T&, const LevelState&){}

static auto render_block(const BackgroundBushState&, const LevelState&){}
static auto render_block(const BackgroundHillState&, const LevelState&){}

static auto render_block(const BlockBase& block, const LevelState& level) -> void{
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

static auto render_block(const BricksBlockState& block, const LevelState& level){
  for (const auto& particle : block.particles){
    render_entity(particle, level);
  }

  render_block(block | util::as<BlockBase>, level);
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

static auto render_block(const CannonState& block, const LevelState& level){
	for (auto& bullet : block.bullet_generator.items){
		render_entity(bullet, level);
	}

	render_block(block | util::as<BlockBase>, level);
}

} //namespace mario::views
