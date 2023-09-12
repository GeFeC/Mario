#pragma once

#include "Renderer/Texture.hpp"
#include "Util/Direction.hpp"

#include <glm/glm.hpp>

namespace mario::renderer{

struct Drawable{
  struct Flip{
    util::Flip horizontal = util::Flip::no_flip();
    util::Flip vertical = util::Flip::no_flip();
  } flip; 

  glm::vec2 position;
  glm::vec2 size;
  Texture const* texture;
  float alpha = 1.f;
  
  bool is_visible = true;
};

struct PlainDrawable{
  glm::vec2 position;
  glm::vec2 size;
  glm::vec4 color;
};

struct RotatableDrawable : Drawable{
  float rotation = 0.f;
};

} //namespace mario::renderer
