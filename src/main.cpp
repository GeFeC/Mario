#include <glad/glad.h>

#include "Renderer/Renderer.hpp"
#include "Window.hpp"
#include "config.hpp"
#include "res/fonts.hpp"
#include "res/textures.hpp"

#include "Controllers/AppController.hpp"
#include "States/AppState.hpp"
#include "Frame.hpp"

#include "Terminal.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

auto main() -> int{
  mario::window::init();
  mario::renderer::init();

  auto app = mario::AppState();
  app.current_frame = mario::AppState::Frame::Level61;

  mario::terminal::run(app);
  mario::window::show();

  while(!mario::window::should_close()){
    mario::app_controller::controller(app);
  }
}
