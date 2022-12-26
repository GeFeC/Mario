#include "ShaderProgram.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <array>
#include <stdexcept>

namespace program = renderer::shader_program;

static GLuint program_id;
static Shader vertex_shader, fragment_shader;

auto program::create_program(String_t& vertex_shader_script, String_t& fragment_shader_script) -> void{
  program_id = glCreateProgram();

  vertex_shader.create(Shader::ShaderType::Vertex, vertex_shader_script);
  fragment_shader.create(Shader::ShaderType::Fragment, fragment_shader_script);

  link_shaders();
  use();
}

auto program::link_shaders() -> void{
  glAttachShader(program_id, vertex_shader.shader_id);
  glAttachShader(program_id, fragment_shader.shader_id);

  glLinkProgram(program_id);

  auto success = GLint{};
  glGetProgramiv(program_id, GL_LINK_STATUS, &success);
  
  if (success){
    return;
  }
  
  static constexpr auto ERROR_MESSAGE_BUFFER_SIZE = 512;
  auto msg = std::array<char, ERROR_MESSAGE_BUFFER_SIZE>{};
  glGetProgramInfoLog(program_id, ERROR_MESSAGE_BUFFER_SIZE, nullptr, msg.data());

  throw std::runtime_error("Failed to link shader with error:\n" + std::string{ msg.data() } );
}

auto program::use() noexcept -> void{
  glUseProgram(program_id);
}

auto program::set_uniform(const std::string& uniform_name, const glm::mat4& matrix) noexcept -> void{
  glUniformMatrix4fv(
    glGetUniformLocation(program_id, uniform_name.c_str()), 
    1, 
    GL_FALSE, 
    glm::value_ptr(matrix)
  );
}

auto program::set_uniform(const std::string& uniform_name, const glm::vec4& vector) noexcept -> void{
  glUniform4fv(
    glGetUniformLocation(program_id, uniform_name.c_str()), 
    1, 
    glm::value_ptr(vector)
  );
}

auto program::set_uniform(const std::string& uniform_name, int value) noexcept -> void{
  glUniform1i(
    glGetUniformLocation(program_id, uniform_name.c_str()), 
    value
  );
}
