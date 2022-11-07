#pragma once

#include <glad/glad.h>

struct Shader{
  using String_t = const char*;

  enum class ShaderType{
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER
  };

  GLuint shader_id;

  auto create(ShaderType type, String_t& script) -> void;
  auto compile() -> void;
};
