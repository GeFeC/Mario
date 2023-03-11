#include <glad/glad.h>

#include "Renderer/ShaderProgram.hpp"
#include "Window.hpp"
#include "config.hpp"
#include "res/fonts.hpp"
#include "res/textures.hpp"

#include "Controllers/AppController.hpp"
#include "States/AppState.hpp"
#include "Frame.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

auto window_resize_callback(GLFWwindow*, int window_width, int window_height){
  const auto ratio = (window_width | util::as<float>) / window_height;

  auto game_screen_width = window_width;
  auto game_screen_height = window_height;

  if (ratio > (config::BlocksInRow | util::as<float>) / config::BlocksInColumn){
    game_screen_width = window_height * config::BlocksInRow / config::BlocksInColumn;
  }
  else{
    game_screen_height = window_width * config::BlocksInColumn / config::BlocksInRow;
  }

  const auto center_x = (window_width - game_screen_width) / 2;
  const auto center_y = (window_height - game_screen_height) / 2;

  glViewport(center_x, center_y, game_screen_width, game_screen_height);
}

auto main() -> int{
  window::init("Super Mario", glm::vec2(config::InitialWindowWidth, config::InitialWindowHeight));
  window::set_resize_callback(window_resize_callback);

  renderer::init();

  auto app = AppState();
  app.current_frame = AppState::Frame::Level12;

  window::show();

  while(!window::should_close()){
    app_controller(app);
  }
}
