#include "Renderer.hpp"

#include "ShaderProgram.hpp"
#include "Vao.hpp"

#include "Shaders/fShader.hpp"
#include "Shaders/vShader.hpp"
#include "config.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace program = renderer::shader_program;

static GLuint shader_program_id;

auto renderer::init() -> void{
  vao::create({
    //vertex:  texture:
    0.f, 0.f, 0.f, 0.f,
    1.f, 0.f, 1.f, 0.f,
    1.f, 1.f, 1.f, 1.f,
    0.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 1.f,
    1.f, 1.f, 1.f, 1.f
  });

  vao::add_attrib_ptr(0, 2, 0);
  vao::add_attrib_ptr(1, 2, 2);

  program::create_program(shaders::v_shader, shaders::f_shader);

  const auto projection_matrix = glm::ortho(
    0.f, 
    config::FrameBufferSize.x, 
    config::FrameBufferSize.y, 
    0.f
  );

  program::set_uniform("projection", projection_matrix);
}

static auto draw_element(const Drawable& drawable, bool is_glyph, const glm::vec2& offset){
  if (!drawable.is_visible) return;

  const auto flip_offset_x = drawable.size.x * (drawable.flip.horizontal - 1) / 2;
  const auto flip_offset_y = drawable.size.y * (drawable.flip.vertical - 1) / 2;

  using renderer::ShadowOffset;
  program::set_uniform("rect", {
    glm::round(drawable.position.x - flip_offset_x + renderer::shadow_mode * ShadowOffset - offset.x),
    glm::round(drawable.position.y - flip_offset_y + renderer::shadow_mode * ShadowOffset - offset.y),
    glm::round(drawable.size.x * drawable.flip.horizontal),
    glm::round(drawable.size.y * drawable.flip.vertical)
  });

  program::set_uniform("is_glyph", is_glyph);
  program::set_uniform("is_shadow", renderer::shadow_mode);
  program::set_uniform("is_highlighted", renderer::highlight_mode);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}

auto renderer::draw(const Drawable& drawable, bool is_glyph) -> void{
  drawable.texture->set_mag_filter(GL_NEAREST);
  drawable.texture->set_min_filter(GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  draw_element(drawable, is_glyph, glm::vec2(0));
}

auto renderer::draw_plain(const PlainDrawable& drawable) -> void{
  program::set_uniform("f_color", drawable.color);
  program::set_uniform("is_texture", 0);

  program::set_uniform("rect", {
    glm::round(drawable.position.x + renderer::shadow_mode * ShadowOffset),
    glm::round(drawable.position.y + renderer::shadow_mode * ShadowOffset),
    glm::round(drawable.size.x),
    glm::round(drawable.size.y)
  });

  program::set_uniform("is_shadow", renderer::shadow_mode);
  program::set_uniform("is_glyph", false);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  program::set_uniform("is_texture", 1);
}

auto renderer::print(const Text& text, const glm::vec2& offset) -> void{
  if (!text.is_visible) return;

  for (int i = 0; i < text.get_length(); ++i){
    auto current_char = text.get_text()[i];

    if (current_char == '\n'){
      continue;
    }

    const auto& current_glyph_texture = text.get_font()->glyphs[current_char].texture;
    
    current_glyph_texture.set_mag_filter(GL_NEAREST);
    current_glyph_texture.set_min_filter(GL_NEAREST_MIPMAP_NEAREST);
    draw_element(text.get_glyph(i), true, offset);
  }
}
