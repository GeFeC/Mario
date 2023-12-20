#pragma once

#include "States/LevelState.hpp"
#include "Views/Blocks.hpp"
#include "Util/Loop.hpp"

namespace mario::views{

template<typename T>
static auto render_background(const T&, const LevelState&) {}

static auto render_background(const LavaState& lava, const LevelState& level){
	util::for_2d([&](auto x, auto y){
		auto drawable = renderer::Drawable{};
		drawable.position = BlockBase::Size 
			* (lava.position + glm::vec2(level.lava_offset + x, y)) 
			- level.camera_offset;

		drawable.size = glm::vec2(BlockBase::Size);

		drawable.texture = y == 0 ? &textures::lava_top : &textures::lava_bottom;
		
		renderer::draw(drawable);
	}, std::make_pair(-1.f, 0.f), std::make_pair(lava.size.x, lava.size.y));
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

  render_block(BlockState({ x, y }, &top_left), level);
  render_block(BlockState({ x, y + 1 }, &bottom_left), level);

  for (int i = 0; i < size; ++i){
    render_block(BlockState({ x + i + 1, y }, &top_center), level);
    render_block(BlockState({ x + i + 1, y + 1 }, &bottom_center), level);
  }

  render_block(BlockState({ x + size + 1, y }, &top_right), level);
  render_block(BlockState({ x + size + 1, y + 1 }, &bottom_right), level);
}

static auto render_background(const BackgroundBushState& bush, const LevelState& level){
  render_block(bush, level);
}

static auto render_background(const BackgroundHillState& bush, const LevelState& level){
  render_block(bush, level);
}

} //namespace mario::views
