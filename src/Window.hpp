#pragma once

#include <string>
#include <glm/glm.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace window{
  inline auto delta_time = 0.f;

  auto init(const std::string& title, const glm::vec2& initial_size) -> void;
  auto show() -> void;
  auto update() -> void;
  auto should_close() -> bool;

  auto set_resize_callback(void callback(GLFWwindow*, int width, int height)) -> void;

  auto is_key_pressed(int key) -> bool;
}
