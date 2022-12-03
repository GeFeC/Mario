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
  static constexpr auto Size = 60;

  Texture* texture;
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
  } bounce_state;

  int hitbox_block_index = -1;
};

struct BlinkingBlockBase{
  int blink_state = 0;
};

struct BackgroundObjectState{
  glm::vec2 position;
  int size;

  BackgroundObjectState(const glm::vec2& position, int size) : position(position), size(size) {}
};

struct BlockState : BlockBase{
  BlockState(const glm::vec2& position, Texture* texture){
    this->position = position * 60.f;
    this->texture = texture;
  }
};

struct BouncingBlockState : BlockBase, BouncingBlockBase{
  BouncingBlockState() = default;
  BouncingBlockState(const glm::vec2& position){
    this->position = position * 60.f;
  }
};

struct CoinBlockState : BlockBase, BlinkingBlockBase{
  CoinBlockState(const glm::vec2& position){
    this->position = position * 60.f;
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
    this->position = position * 60.f;

    for (auto& particle : particles){
      particle.position = this->position + size / 2.f - particle.size / 2.f;
    }
  }
};

struct QBlockState : BouncingBlockState, BlinkingBlockBase{
  QBlockState(const glm::vec2& position){
    this->position = position * 60.f;
    this->texture = &textures::q_block[0];
  }
};

struct SpinningCoinState : BouncingBlockState{
  SpinningCoinState(const glm::vec2& position){
    this->position = position * 60.f;
    this->texture = &textures::spinning_coin[0];
  }
};
