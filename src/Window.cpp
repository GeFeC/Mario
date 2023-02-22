#include "Window.hpp"

#include <stdexcept>

static GLFWwindow* g_window;

auto window::init(const std::string& title, const glm::vec2& initial_size) -> void{
  if (!glfwInit()){
    throw std::runtime_error("Unable to initialise glfw!");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  g_window = glfwCreateWindow(initial_size.x, initial_size.y, title.c_str(), nullptr, nullptr);

  if (g_window == nullptr){
    throw std::runtime_error("Unable to create window!");
  }

  glfwMakeContextCurrent(g_window);
  
  //Center window:
  const auto monitor = glfwGetPrimaryMonitor();
  const auto video_mode = glfwGetVideoMode(monitor);

  const auto centered_pos = glm::vec2(
    video_mode->width / 2 - initial_size.x / 2,
    video_mode->height / 2 - initial_size.y / 2
  );

  glfwSetWindowPos(g_window, centered_pos.x, centered_pos.y);

  //Resize callback


  //Initialise glad

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    throw std::runtime_error("Unable to initialise glad!");
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   
}

auto window::set_resize_callback(void callback(GLFWwindow*, int width, int height)) -> void{
  glfwSetFramebufferSizeCallback(g_window, callback);
}

auto window::show() -> void{
  glfwShowWindow(g_window);
}

auto window::should_close() -> bool{
  return glfwWindowShouldClose(g_window);
}

auto window::update() -> void{
  //VSync on:
  glfwSwapInterval(1);
  glfwSwapBuffers(g_window);
  glfwPollEvents();
}

auto window::is_key_pressed(int key) -> bool{
  return glfwGetKey(g_window, key) == GLFW_PRESS;
}
