#include "Text.hpp"

#include <algorithm>

Text::Text() {
  size = { 0.f, 0.f };
}

Text::Text(Font *font, const std::string& label) {
  set_font(font);
  set_text(label);
}

auto Text::set_font(Font* font)  -> void{
  this->font = font;

  font_size = font->size;

  const auto glyph_with_largest_bearing = std::max_element(
    font->glyphs.begin(), 
    font->glyphs.end(), 
    [](const auto& lhs, const auto& rhs){
      return lhs.bearing.y < rhs.bearing.y;
    }
  );

  largest_glyph_bearing_y = glyph_with_largest_bearing->bearing.y;
}

auto Text::set_text(const std::string& text)  -> void{
  this->text = text;
  glyphs.resize(text.size());

  for (size_t i = 0; i < text.size(); ++i){
    if (text[i] == '\n'){
      continue;
    }

    auto& current_font_glyph = font->glyphs[text[i]];
    auto& current_text_glyph = glyphs[i];

    current_text_glyph = create_glyph_from_char_(text[i]);
  }

  refresh();
}

auto Text::create_glyph_from_char_(char character) const  -> Glyph{
  auto new_text_glyph = Glyph{};
  auto& font_glyph = font->glyphs[character];

  new_text_glyph.size = glm::round(font_glyph.size);

  if (character == ' '){
    new_text_glyph.size.x += font_glyph.advance >> 6;
  }

  new_text_glyph.bearing = font_glyph.bearing;
  new_text_glyph.advance = static_cast<int>(font_glyph.advance);

  return new_text_glyph;
}

auto Text::refresh()  -> void{
  set_position(position);
  set_color(color);
}

auto Text::set_position(const glm::vec2& position)  -> void{
  set_position_for_every_glyph_(position);
  this->position = position;
}

static auto get_lines_count(const std::string& text){
  return (1 + std::count(text.begin(), text.end(), '\n'));
}

auto Text::set_position_for_every_glyph_(const glm::vec2& position)  -> void{
  auto y = position.y;
  auto x = position.x;

  for (int i = 0; i < glyphs.size(); ++i){
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

    x += static_cast<float>(glyph.advance >> 6);
  }

  size.x = get_text_width_();
  size.y = largest_glyph_bearing_y * get_lines_count(text);
}

auto Text::get_text_width_() -> float{
  if (glyphs.size() == 0){
    return 0.f;
  }

  const auto last_glyph_x = glyphs.back().position.x;
  const auto last_glyph_width = glyphs.back().size.x;
  const auto first_glyph_x = glyphs.front().position.x;

  return last_glyph_x + last_glyph_width - first_glyph_x;
}

auto Text::set_color(const glm::vec4& color)  -> void{
  this->color = color;
  for (auto& glyph : glyphs){
    glyph.color = color;
  }
}

auto Text::get_color() const -> const glm::vec4&{
  return color;
}

auto Text::get_size() const -> const glm::vec2&{
  return size;
}

auto Text::get_position() const -> const glm::vec2{
  return position;
}

auto Text::get_text() const -> const std::string&{
  return text;
}

auto Text::get_glyph(int index) const -> const Glyph&{
  return glyphs[index];
}

auto Text::get_length() const -> int{
  return glyphs.size();
}

auto Text::get_font() const -> Font*{
  return font;
}

auto Text::get_total_height() const -> float{
  return get_lines_count(text) * font->size;
}
