#pragma once

#include "Window.hpp"
#include "Renderer/Texture.hpp"
#include "States/AppState.hpp"

#include <vector>

namespace mario::frames{

template<typename SetupFunction, typename LoopFunction>
static auto run_frame(
    AppState& app,
    AppState::Frame frame, 
    const std::vector<renderer::TextureGroup>& textures, 
    const SetupFunction& setup, 
    const LoopFunction& loop
){
  setup(app);

  for (auto texture : textures){
    texture.allocate();
  }  

	const auto should_end_frame = [&]{
		return 
			window::should_close() ||
			app.current_frame != frame ||
			app.should_restart_current_frame ||
			app.should_exit;
	};

  while(!should_end_frame()){
    const auto frame_start_time = glfwGetTime();
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);

    loop(app);

    window::update();
    window::delta_time = std::min(glfwGetTime() - frame_start_time, 1.0 / 30.0); //Min 30 FPS
  }

  app.should_restart_current_frame = false;

  for (auto texture : textures){
    texture.free();
  }
}

} //namespace mario::frames
