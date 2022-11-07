#include <glad/glad.h>

#include "Renderer/ShaderProgram.hpp"
#include "Window.hpp"
#include "config.hpp"
#include "res/fonts.hpp"
#include "res/textures.hpp"

#include "Controllers/AppController.hpp"
#include "States/AppState.hpp"
#include "Views/App.hpp"
#include "Frame.hpp"

#include "Frames/Level11.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

auto window_resize_callback(GLFWwindow*, int width, int height){
  const auto ratio = static_cast<float>(width) / height;

  auto new_width = width;
  auto new_height = height;

  if (ratio > static_cast<float>(config::BlocksInRow) / config::BlocksInColumn){
    new_width = height * config::BlocksInRow / config::BlocksInColumn;
  }
  else{
    new_height = width * config::BlocksInColumn / config::BlocksInRow;
  }

  glViewport((width - new_width) / 2, (height - new_height) / 2, new_width, new_height);
}

auto main() -> int{
  window::init("Super Mario", glm::vec2(config::InitialWindowWidth, config::InitialWindowHeight));
  window::set_resize_callback(window_resize_callback);

  renderer::init();

  auto app_state = AppState();
  app_state.current_frame = AppState::Frame::Level11;

  window::show();

  while(!window::should_close()){
    auto& frame = app_state.current_frame;

    if (frame == AppState::Frame::Level11){
      run_frame_level11(app_state);

      const auto player_hp = app_state.current_level_state.stats_state.hp - 1;

      if (player_hp == 0){
        break;
      }

      app_state.current_level_state = LevelState{};
      app_state.current_level_state.stats_state.hp = player_hp;
    }
  }
}
