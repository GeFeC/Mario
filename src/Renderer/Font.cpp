#include "Font.hpp"
#include "config.hpp"
#include "Util.hpp"

#include <stdexcept>

Font::Font(const std::string& path) noexcept{
  source_path = config::FontsSourceDir + path;
}

auto Font::allocate(int font_size, int glyphs_count) -> void{
  size = font_size;

  FT_Library ft;
  if (FT_Init_FreeType(&ft)){
    throw std::runtime_error("Failed to initialise FT_Library!");
  }

  FT_Face face;
  if (FT_New_Face(ft, source_path.c_str(), 0, &face)){
    throw std::runtime_error("Failed to load font from: " + source_path);
  }

  FT_Set_Pixel_Sizes(face, 0, font_size);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  for (int c = 0; c < glyphs_count; c++){
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)){
      throw std::runtime_error("Failed to load glyph: " + std::string{ c | util::as<char> });
    }

    const auto texture_id = create_texture_from_face_(face);
    auto& current_glyph = glyphs.emplace_back();

    current_glyph.size = { face->glyph->bitmap.width, face->glyph->bitmap.rows };
    current_glyph.bearing = { face->glyph->bitmap_left, face->glyph->bitmap_top };
    current_glyph.advance = face->glyph->advance.x;
    current_glyph.texture.id = texture_id;
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

auto Font::create_texture_from_face_(const FT_Face& face) noexcept -> GLuint{
  GLuint texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RED,
    face->glyph->bitmap.width,
    face->glyph->bitmap.rows,
    0,
    GL_RED,
    GL_UNSIGNED_BYTE,
    face->glyph->bitmap.buffer
  );

  glGenerateMipmap(GL_TEXTURE_2D);

  return texture_id;
}

auto Font::free() noexcept -> void{
  for (auto& glyph : glyphs){
    glyph.texture.free();
  }
}
