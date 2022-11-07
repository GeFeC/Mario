#pragma once

#include "Renderer/Texture.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <vector>

class Font{
private:
  struct Glyph{
    Texture texture;
    glm::vec2 size;
    glm::vec2 bearing;
    int advance;
  };

  std::string source_path;
  auto create_texture_from_face_(const FT_Face& face) noexcept -> GLuint;

public:
  inline static constexpr auto DefaultSize = 20;

  int size = 0;

  std::vector<Glyph> glyphs;

  Font(const std::string& path) noexcept;

  auto allocate(int font_size, int glyphs_count = 128) -> void;
  auto free() noexcept -> void;
};
