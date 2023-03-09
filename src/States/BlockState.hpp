#pragma once

#include "Renderer/Texture.hpp"
#include "Renderer/Text.hpp"

#include "States/EntityState.hpp"

#include "res/textures.hpp"
#include "config.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <array>

struct BlockBase{
  Texture const* texture;
  glm::vec2 position = { 0.f, 0.f };
  glm::vec2 size = glm::vec2(config::BlockSize);

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
    this->position = position * config::BlockSize;
    this->texture = texture;
  }
};

struct BouncingBlockState : BlockBase, BouncingBlockBase{
  BouncingBlockState() = default;
  BouncingBlockState(const glm::vec2& position){
    this->position = position * config::BlockSize;
  }
};

struct CoinBlockState : BlockBase, BlinkingBlockBase{
  CoinBlockState(const glm::vec2& position){
    this->position = position * config::BlockSize;
    this->texture = &textures::coin[0];
  }
};

struct BricksBlockState : BouncingBlockState{
  static constexpr auto ParticlesCount = 10;
  static constexpr auto ParticleSize = config::BlockSize / 1.5;

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

  BricksBlockState(const glm::vec2& position) : BricksBlockState(){
    this->position = position * config::BlockSize;

    for (auto& particle : particles){
      const auto center_position = this->position + size / 2.f - particle.size / 2.f;
      particle.position = center_position;
    }
  }
};

struct SpinningCoinState : BouncingBlockState{
  SpinningCoinState(const glm::vec2& position){
    this->position = position * config::BlockSize;
    texture = &textures::spinning_coin[0];
    bounce_state.initial_power = -20.f;
  }
};

using CloudState = std::pair<glm::vec2, int>;
