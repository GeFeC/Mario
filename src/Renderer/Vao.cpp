#include "Vao.hpp"

static constexpr int DEFAULT_VAO_STRIDE = 4;

namespace vao = renderer::vao;

static GLuint vao_id;
static GLuint vbo_id;
static int stride = 4;

auto vao::create(const std::initializer_list<float>& buffer) noexcept -> void{
  auto buffer_vec = std::vector<float>(buffer);

  glGenVertexArrays(1, &vao_id);
  glBindVertexArray(vao_id);

  glGenBuffers(1, &vbo_id);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

  glBufferData(
    GL_ARRAY_BUFFER, 
    buffer_vec.size() * sizeof(float), 
    buffer_vec.data(), GL_STATIC_DRAW
  );
}

auto vao::add_attrib_ptr(GLuint index, GLuint size, GLuint position) noexcept -> void{
  glVertexAttribPointer(
    index, 
    size, 
    GL_FLOAT, 
    GL_FALSE, 
    stride * sizeof(float), 
    (void*)(position * sizeof(float))
  );

  glEnableVertexAttribArray(index);
}
