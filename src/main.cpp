#include "Renderer/Renderer.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Saves.hpp"
#include "States/AppState.hpp"
#include "Controllers/AppController.hpp"

#include "Terminal.hpp"

auto main() -> int{
	mario::audio::init();
	mario::sounds::init();
  mario::window::init();
	mario::input::init();
  mario::renderer::init();

	mario::fonts::normal.allocate(16);

	if (mario::saves::file_exists()){
		const auto controls = mario::saves::load().controls;
		std::transform(controls.begin(), controls.end(), mario::input::controls.begin(), [](int code){
			return mario::input::Key(code);
		});
	}

  auto app = mario::AppState();
  app.current_frame = mario::AppState::Frame::Menu;

  mario::terminal::run(app);
  mario::window::show();

  while(!mario::window::should_close() && !app.should_exit){
    mario::app_controller::run(app);
  }

	mario::audio::free();
	mario::fonts::normal.free();
	mario::renderer::free();
	mario::window::free();

	return 0;
}
