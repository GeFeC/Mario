#pragma once

#include "Renderer/Texture.hpp"
#include "Renderer/Text.hpp"

#include "States/EntityState.hpp"

#include "res/textures.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <array>

struct BlockBase{
  inline static constexpr auto Size = 96.f;

  Texture const* texture;
  glm::vec2 position = { 0.f, 0.f };
  glm::vec2 size = glm::vec2(Size);

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

struct BlinkingBlockBase{};

struct BlockState : BlockBase{
  BlockState(const glm::vec2& position, Texture const* texture){
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

struct CoinBlockState : BlockBase, BlinkingBlockBase{
  explicit CoinBlockState(const glm::vec2& position){
    this->position = position * BlockBase::Size;
    this->texture = &textures::coin[0];
  }
};

struct BricksBlockState : BouncingBlockState{
  static constexpr auto ParticlesCount = 10;
  static constexpr auto ParticleSize = BlockBase::Size / 1.5;

  std::array<EntityState, ParticlesCount> particles;  
  bool are_particles_active = false;

  BricksBlockState(){
    this->texture = &textures::bricks;

    for (auto& particle : particles){
      particle.is_visible = false;
      particle.current_texture = &textures::bricks_particles_normal;
      particle.size = glm::vec2(ParticleSize);
      particle.should_collide = false;
    }
  }

  explicit BricksBlockState(const glm::vec2& position) : BricksBlockState(){
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

struct BackgroundHillState : BlockState{
  explicit BackgroundHillState(const glm::vec2& position, Texture const* texture)
    : BlockState(position, texture) {}
};
struct BackgroundBushState : BlockState{
  explicit BackgroundBushState(const glm::vec2& position, Texture const* texture)
    : BlockState(position, texture) {}
};
