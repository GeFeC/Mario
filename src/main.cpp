#include "Renderer/Renderer.hpp"
#include "Window.hpp"

#include "Controllers/AppController.hpp"
#include "States/AppState.hpp"

#include "Terminal.hpp"

auto main() -> int{
  mario::window::init();
	mario::input::init();
  mario::renderer::init();

	const auto controls = mario::saves::load().controls;
	std::transform(controls.begin(), controls.end(), mario::input::controls.begin(), [](int code){
		return mario::input::Key(code);
	});

  auto app = mario::AppState();
  app.current_frame = mario::AppState::Frame::Menu;

  mario::terminal::run(app);
  mario::window::show();

  while(!mario::window::should_close() && !app.should_exit){
    mario::app_controller::run(app);
  }
}
