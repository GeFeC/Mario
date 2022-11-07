#pragma once

#include "Drawable.hpp"
#include "Text.hpp"

namespace renderer{
  inline bool shadow_mode = false;

  auto init() -> void;
  auto draw(const Drawable& drawable, bool is_glyph = false) noexcept -> void;
  auto print(const Text& text, float offset_x = 0.f) -> void;

  template<typename F>
  auto draw_with_shadow(const F& callable){
    shadow_mode = true;
    callable();
    shadow_mode = false;
    callable();
  }
}
