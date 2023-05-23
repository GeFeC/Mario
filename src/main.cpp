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

auto main() -> int{
  window::init();
  renderer::init();

  auto app = AppState();
  app.current_frame = AppState::Frame::Level33;

  window::show();

  while(!window::should_close()){
    app_controller(app);
  }
}
