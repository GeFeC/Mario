#pragma once

#include "Renderer/Drawable.hpp"
#include "Renderer/Font.hpp"
#include "Util/Cast.hpp"

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <algorithm>

namespace mario::renderer{

class Text :public Drawable{
private:
  struct Glyph : Drawable{
    glm::vec4 color;
    glm::vec2 bearing;
    int advance;
  };

  std::vector<Glyph> glyphs;
  
  float largest_glyph_bearing_y = 0.f;
  float font_size = 0.f;

  static auto get_lines_count(const std::string& text){
    return (1 + std::count(text.begin(), text.end(), '\n'));
  }

public:
  std::string text;
  glm::vec4 color;
  Font* font;

  float font_scale = 0.f;

  Text(){
    size = { 0.f, 0.f };
  }

  Text(Font* font, const std::string& label, float scale = 1.f){
    position = { 0, 0 };

    font_scale = scale;
    this->font = font;
    text = label;

    update_font();
  }

  const auto& get_size() const{
    return size;
  }

  const auto& get_glyph(int index) const{
    return glyphs[index];
  }

  auto get_length() const{
    return glyphs.size();
  }

  auto get_all_lines_height() const{
    return get_lines_count(text) * font->size;
  }

  auto get_text_width() const{
    if (glyphs.size() == 0){
      return 0.f;
    }

    const auto last_glyph_x = glyphs.back().position.x;
    const auto last_glyph_width = glyphs.back().size.x;
    const auto first_glyph_x = glyphs.front().position.x;

    return last_glyph_x + last_glyph_width - first_glyph_x;
  }

  auto update_font() -> void{
    font_size = font->size * font_scale;

    const auto glyph_with_largest_bearing = std::max_element(
      font->glyphs.begin(), 
      font->glyphs.end(), 
      [](const auto& lhs, const auto& rhs){
        return lhs.bearing.y < rhs.bearing.y;
      }
    );

    largest_glyph_bearing_y = glyph_with_largest_bearing->bearing.y * font_scale;
  }

  auto update_position(){
    auto x = position.x;
    auto y = position.y;

    for (auto i = std::size_t(0); i < glyphs.size(); ++i){
      auto& glyph = glyphs[i];

      if (text[i] == '\n'){
        x = position.x;
        y += font_size;

        glyph.position.x = x;
        glyph.position.y = y;
        continue;
      }

      const auto new_glyph_position = glm::vec2(
        x + glyph.bearing.x,
        y - glyph.bearing.y + largest_glyph_bearing_y
      );

      glyph.position = glm::round(new_glyph_position);

      x += (glyph.advance >> 6) | util::as<float>;
    }
  }

  auto update(){
    //Generate glyphs faces and sizes:
    glyphs.resize(text.size());
    
    for (std::size_t i = 0; i < text.size(); ++i){
      if (text[i] == '\n'){
        continue;
      }

      auto new_text_glyph = Glyph{};

      auto character = text[i];
      auto& font_glyph = font->glyphs[character];

      new_text_glyph.size = glm::round(font_glyph.size * font_scale);

      if (character == ' '){
        new_text_glyph.size.x += (font_glyph.advance >> 6);
      }

      new_text_glyph.bearing = font_glyph.bearing * font_scale;
      new_text_glyph.advance = (font_glyph.advance * font_scale) | util::as<int>;

      glyphs[i] = new_text_glyph;
    }

    update_position();

    //Set text size
    size.x = get_text_width();
    size.y = largest_glyph_bearing_y * get_lines_count(text);

    //Set text color
    for (auto& glyph : glyphs){
      glyph.color = color;
    }
  }
};

} //namespace mario::renderer
