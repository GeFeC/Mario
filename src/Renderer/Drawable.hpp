#pragma once

#include "Renderer/Texture.hpp"

#include <glm/glm.hpp>

struct Drawable{
  glm::vec2 position;
  glm::vec2 size;
  Texture const* texture;

  struct Flip{
    static constexpr auto UseFlip = -1;
    static constexpr auto NoFlip = 1;

    int horizontal = NoFlip;
    int vertical = NoFlip;
  } flip;

  bool is_visible = true;
};

struct PlainDrawable{
  glm::vec2 position;
  glm::vec2 size;
  glm::vec4 color;
};
