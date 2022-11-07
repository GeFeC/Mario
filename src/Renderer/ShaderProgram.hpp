#pragma once

#include "Shader.hpp"

#include <glm/glm.hpp>

#include <string>

namespace renderer::shader_program{
  using String_t = Shader::String_t;

  auto use() noexcept -> void;
  auto link_shaders() -> void;

  auto create_program(String_t& vertex_shader_script, String_t& fragment_shader_script) -> void;

  auto set_uniform(const std::string& uniform_name, const glm::mat4& matrix) noexcept -> void;
  auto set_uniform(const std::string& uniform_name, const glm::vec4& vector) noexcept -> void;
  auto set_uniform(const std::string& uniform_name, int flag) noexcept -> void;
}
