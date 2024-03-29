#pragma once

#include "Renderer/Texture.hpp"
#include "States/EntityState.hpp"
#include "res/textures.hpp"

#include <glm/glm.hpp>
#include <array>

namespace mario{

struct BlockBase{
  inline static constexpr auto Size = 96.f;

  renderer::Texture const* texture;
  glm::vec2 position = { 0.f, 0.f };
  glm::vec2 size = glm::vec2(Size);
  float alpha = 1.f;

  bool is_visible = true;
  bool is_solid = true;
};

struct BouncingBlockBase{
  struct BounceState{
    bool is_bouncing = false;
    bool can_bounce = true;

    int hits_required_to_bounce = 0;
    int bounces_count = 1;

    float initial_power = -7.f;
    float power = 0.f;
    float temp_y = 0.f;
  } bounce_state{};

  int hitbox_block_index = -1;
};

struct BlockState : BlockBase{
  BlockState(const glm::vec2& position, renderer::Texture const* texture){
    this->position = position * BlockBase::Size;
    this->texture = texture;
  }
};

struct BouncingBlockState : BlockBase, BouncingBlockBase{
  BouncingBlockState() = default;
  explicit BouncingBlockState(const glm::vec2& position){
    this->position = position * BlockBase::Size;
  }
};

struct CoinBlockState : BlockBase{
  explicit CoinBlockState(const glm::vec2& position){
    this->position = position * BlockBase::Size;
    this->texture = &textures::coin[0];
  }
};

struct PurpleCoinBlockState : CoinBlockState{
  explicit PurpleCoinBlockState(const glm::vec2& position) : CoinBlockState(position) {}
};

struct BricksBlockState : BouncingBlockState{
  static constexpr auto ParticlesCount = 10;
  static constexpr auto ParticleSize = BlockBase::Size / 1.5;

  std::array<EntityState, ParticlesCount> particles;  
  bool are_particles_active = false;

  explicit BricksBlockState(const renderer::TextureGroup& bricks_textures){
    this->texture = &bricks_textures[0];

    for (auto& particle : particles){
      particle.is_visible = false;
      particle.current_texture = &bricks_textures[1];
      particle.size = glm::vec2(ParticleSize);
      particle.should_collide = false;
    }
  }

  BricksBlockState(const renderer::TextureGroup& bricks_textures, const glm::vec2& position) 
    : BricksBlockState(bricks_textures){
    this->position = position * BlockBase::Size;

    for (auto& particle : particles){
      const auto center_position = this->position + size / 2.f - particle.size / 2.f;
      particle.position = center_position;
    }
  }
};

struct SpinningCoinState : BouncingBlockState{
  explicit SpinningCoinState(const glm::vec2& position){
    this->position = position * BlockBase::Size;
    texture = &textures::spinning_coin[0];
    bounce_state.initial_power = -20.f;
    is_visible = false;
  }
};

struct CloudState{
  glm::vec2 position;
  int size;
};

struct UnstableCloudState : BlockState{
  UnstableCloudState(const glm::vec2& position) : BlockState(position, &textures::cloud_unstable) {}
};

struct BackgroundHillState : BlockState{
  explicit BackgroundHillState(const glm::vec2& position, renderer::Texture const* texture)
    : BlockState(position, texture) {}
};

struct BackgroundBushState : BlockState{
  explicit BackgroundBushState(const glm::vec2& position, renderer::Texture const* texture)
    : BlockState(position, texture) {}
};

} //namespace mario
