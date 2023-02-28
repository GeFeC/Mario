#include "Shader.hpp"
#include "Util/Util.hpp"

#include <array>
#include <vector>
#include <string>
#include <stdexcept>

auto Shader::create(ShaderType type, String_t& script) -> void{
  shader_id = glCreateShader(type | util::as<int>); 
  glShaderSource(shader_id, 1, &script, nullptr);
  
  compile();
}

auto Shader::compile() -> void{
  glCompileShader(shader_id);

  auto success = int{};
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  if (success) {
    return;
  }

  static constexpr auto ERROR_MESSAGE_BUFFER_SIZE = 512;

  auto msg = std::array<char, ERROR_MESSAGE_BUFFER_SIZE>{};
  glGetShaderInfoLog(shader_id, ERROR_MESSAGE_BUFFER_SIZE, NULL, msg.data());
  
  throw std::runtime_error("Failed to compile shader with error:\n" + std::string{ msg.data() });
}
