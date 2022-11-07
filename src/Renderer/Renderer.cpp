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

  const auto projection_matrix = glm::ortho(0.f, 0.f + config::InitialWindowWidth, 0.f + config::InitialWindowHeight, 0.f);
  program::set_uniform("projection", projection_matrix);
}

static auto draw_element(const Drawable& drawable, bool is_glyph, float offset_x = 0.f){
  if (!drawable.is_visible) return;

  const auto flip_offset_x = drawable.size.x * (drawable.flip - 1) / 2;

  program::set_uniform("rect", {
    std::round(drawable.position.x - flip_offset_x + renderer::shadow_mode * 5 - offset_x),
    std::round(drawable.position.y + renderer::shadow_mode * 5),
    std::round(drawable.size.x * drawable.flip),
    std::round(drawable.size.y)
  });

  program::set_uniform("is_glyph", is_glyph);
  program::set_uniform("is_shadow", renderer::shadow_mode);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}

auto renderer::draw(const Drawable& drawable, bool is_glyph) noexcept -> void{
  drawable.texture->bind();
  drawable.texture->set_mag_filter(GL_NEAREST);

  draw_element(drawable, is_glyph);
}

auto renderer::print(const Text& text, float offset_x) -> void{
  if (!text.is_visible) return;

  for (int i = 0; i < text.get_length(); ++i){
    auto current_char = text.get_text()[i];

    if (current_char == '\n'){
      continue;
    }

    const auto& current_glyph_texture = text.get_font()->glyphs[current_char].texture;
    
    current_glyph_texture.bind();
    current_glyph_texture.set_mag_filter(GL_NEAREST);

    draw_element(text.get_glyph(i), true, offset_x);
  }
}
