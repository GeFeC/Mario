#pragma once

#include <glad/glad.h>
#include <vector>

namespace renderer::vao{
  auto create(const std::initializer_list<float>& buffer) noexcept -> void;
  auto add_attrib_ptr(GLuint index, GLuint size, GLuint position) noexcept -> void;
}
