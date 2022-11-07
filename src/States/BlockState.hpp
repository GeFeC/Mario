#pragma once

#include "Renderer/Texture.hpp"
#include "Renderer/Text.hpp"

#include <glm/glm.hpp>
#include <vector>

struct BlockState{
  static constexpr auto Size = 60;

  enum class Type{
    Solid,
    Bricks,
    Coin,
    QBlock,
    SpinningCoin
  };
  
  struct BounceState{
    bool is_bouncing = false;
    bool can_bounce = true;

    int hits_required_to_bounce = 0;
    int bounces_count = 1;

    float initial_power = -7.f;
    float power = 0.f;
    float temp_y = 0.f;
  } bounce_state;

  std::vector<Type> types;

  Texture* texture;
  glm::vec2 position = { 0.f, 0.f };
  glm::vec2 size = glm::vec2(Size);
  
  int blink_state = 0;
  bool is_visible = true;

  BlockState(const glm::vec2& position, Texture* texture, const std::vector<Type>& types = {}) 
  : position(position), texture(texture), types(types) {}
};
