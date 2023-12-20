#include "Renderer/Renderer.hpp"
#include "Window.hpp"

#include "Controllers/AppController.hpp"
#include "States/AppState.hpp"

#include "Terminal.hpp"

auto main() -> int{
  mario::window::init();
  mario::renderer::init();

  auto app = mario::AppState();
  app.current_frame = mario::AppState::Frame::Level72;

  mario::terminal::run(app);
  mario::window::show();

  while(!mario::window::should_close()){
    mario::app_controller::run(app);
  }
}
