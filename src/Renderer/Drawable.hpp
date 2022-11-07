#pragma once

#include "Renderer/Texture.hpp"

#include <glm/glm.hpp>

struct Drawable{
  glm::vec2 position;
  glm::vec2 size;
  Texture* texture;
  int flip = 1;
  bool is_visible = true;
};
