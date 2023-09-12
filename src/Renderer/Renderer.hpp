#pragma once

#include "Shaders/VertexShader.hpp"
#include "Shaders/FragmentShader.hpp"
#include "Drawable.hpp"
#include "Text.hpp"
#include "Util/Cast.hpp"
#include "config.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>

namespace mario::renderer{

enum class ShaderType{
  Vertex = GL_VERTEX_SHADER,
  Fragment = GL_FRAGMENT_SHADER
};

using ShaderScript = const char*;
static GLuint vertex_shader, fragment_shader;
static GLuint shader_program;

static auto create_shader(ShaderScript script, ShaderType type){
  const auto shader = glCreateShader(type | util::as<int>); 
  glShaderSource(shader, 1, &script, nullptr);
  glCompileShader(shader);

  auto success = int{};
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success) {
    return shader;
  }

  static constexpr auto ErrorMessageBufferSize = 512;

  auto msg = std::array<char, ErrorMessageBufferSize>{};
  glGetShaderInfoLog(shader, ErrorMessageBufferSize, NULL, msg.data());
  
  throw std::runtime_error("Failed to compile shader with error:\n" + std::string{ msg.data() });

  return 0u;
}

static auto set_uniform(const std::string& uniform_name, const glm::mat4& matrix) noexcept -> void{
  glUniformMatrix4fv(
    glGetUniformLocation(shader_program, uniform_name.c_str()), 
    1, 
    GL_FALSE, 
    glm::value_ptr(matrix)
  );
}

static auto set_uniform(const std::string& uniform_name, const glm::vec4& vector) noexcept -> void{
  glUniform4fv(
    glGetUniformLocation(shader_program, uniform_name.c_str()), 
    1, 
    glm::value_ptr(vector)
  );
}

static auto set_uniform(const std::string& uniform_name, int value) noexcept -> void{
  glUniform1i(
    glGetUniformLocation(shader_program, uniform_name.c_str()), 
    value
  );
}

static auto set_uniform(const std::string& uniform_name, float value) noexcept -> void{
  glUniform1f(
    glGetUniformLocation(shader_program, uniform_name.c_str()), 
    value
  );
}

static GLuint vao, vbo;
static constexpr auto VaoStride = 4;

static auto vao_add_attrib_ptr(GLuint index, GLuint size, GLuint position) noexcept -> void{
  glVertexAttribPointer(
    index, 
    size, 
    GL_FLOAT, 
    GL_FALSE, 
    VaoStride * sizeof(float), 
    (void*)(position * sizeof(float))
  );

  glEnableVertexAttribArray(index);
}

static auto init(){
  //Create VAO
  auto buffer_vec = std::array{
    0.f, 0.f, 0.f, 0.f,
    1.f, 0.f, 1.f, 0.f,
    1.f, 1.f, 1.f, 1.f,
    0.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 1.f,
    1.f, 1.f, 1.f, 1.f
  };

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(
    GL_ARRAY_BUFFER, 
    buffer_vec.size() * sizeof(float), 
    buffer_vec.data(), GL_STATIC_DRAW
  );

  vao_add_attrib_ptr(0, 2, 0);
  vao_add_attrib_ptr(1, 2, 2);

  //Create ShaderProgram
  vertex_shader = create_shader(vertex_shader_script, ShaderType::Vertex);
  fragment_shader = create_shader(fragment_shader_script, ShaderType::Fragment);

  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);

  glLinkProgram(shader_program);

  auto success = GLint{};
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  
  if (!success){
    static constexpr auto ErrorMessageBufferSize = 512;
    auto msg = std::array<char, ErrorMessageBufferSize>{};
    glGetProgramInfoLog(shader_program, ErrorMessageBufferSize, nullptr, msg.data());

    throw std::runtime_error("Failed to link shader with error:\n" + std::string{ msg.data() } );
  }
  glUseProgram(shader_program);

  //Create Projection Matrix
  const auto projection_matrix = glm::ortho(
    0.f, 
    config::FrameBufferSize.x, 
    config::FrameBufferSize.y, 
    0.f
  );

  set_uniform("projection", projection_matrix);
}

static constexpr auto ShadowOffset = 8.f;
inline bool shadow_mode = false;
inline bool highlight_mode = false;

static auto draw_base(const Drawable& drawable, bool is_glyph, const glm::vec2& offset){
  if (!drawable.is_visible) return;

  const auto flip_offset_x = drawable.size.x * (drawable.flip.horizontal.as_binary() - 1);
  const auto flip_offset_y = drawable.size.y * (drawable.flip.vertical.as_binary() - 1);

  using renderer::ShadowOffset;
  set_uniform("rect", {
    glm::round(drawable.position.x - flip_offset_x + renderer::shadow_mode * ShadowOffset - offset.x),
    glm::round(drawable.position.y - flip_offset_y + renderer::shadow_mode * ShadowOffset - offset.y),
    glm::round(drawable.size.x * drawable.flip.horizontal.as_int()),
    glm::round(drawable.size.y * drawable.flip.vertical.as_int())
  });

  set_uniform("texture_alpha", drawable.alpha);
  set_uniform("is_glyph", is_glyph);
  set_uniform("is_shadow", renderer::shadow_mode);
  set_uniform("is_highlighted", renderer::highlight_mode);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}

static auto draw(const Drawable& drawable, bool is_glyph = false){
  drawable.texture->set_mag_filter(GL_NEAREST);
  drawable.texture->set_min_filter(GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  draw_base(drawable, is_glyph, glm::vec2(0));
}

static auto draw(const RotatableDrawable& drawable){
  auto rotation_matrix = glm::mat4(1.0f);
  rotation_matrix = glm::translate(rotation_matrix, glm::vec3(0.5f, 0.5f, 0.f));
  rotation_matrix = glm::rotate(rotation_matrix, drawable.rotation, glm::vec3(0.f, 0.f, 1.f));
  rotation_matrix = glm::translate(rotation_matrix, glm::vec3(-0.5f, -0.5f, 0.f));

  set_uniform("rotation", rotation_matrix);

  draw(drawable | util::as<Drawable>);

  set_uniform("rotation", glm::mat4(1.0f));
}

static auto draw_plain(const PlainDrawable& drawable){
  set_uniform("f_color", drawable.color);
  set_uniform("is_texture", 0);

  set_uniform("rect", {
    glm::round(drawable.position.x + renderer::shadow_mode * ShadowOffset),
    glm::round(drawable.position.y + renderer::shadow_mode * ShadowOffset),
    glm::round(drawable.size.x),
    glm::round(drawable.size.y)
  });

  set_uniform("is_shadow", renderer::shadow_mode);
  set_uniform("is_glyph", false);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  set_uniform("is_texture", 1);
}

static auto print(const Text& text, const glm::vec2& offset){
  if (!text.is_visible) return;

  for (auto i = std::size_t(0); i < text.get_length(); ++i){
    auto current_char = text.text[i];

    if (current_char == '\n'){
      continue;
    }

    const auto& current_glyph_texture = text.font->glyphs[current_char].texture;
    
    current_glyph_texture.set_mag_filter(GL_NEAREST);
    current_glyph_texture.set_min_filter(GL_NEAREST_MIPMAP_NEAREST);
    draw_base(text.get_glyph(i), true, offset);
  }
}

template<typename F>
auto draw_with_shadow(const F& callable){
  shadow_mode = true;
  callable();
  shadow_mode = false;
  callable();
}

} //namespace mario::renderer
