#pragma once

#include "Window.hpp"
#include "Renderer/Texture.hpp"
#include "States/AppState.hpp"

#include <vector>

namespace mario::frames{

template<typename SetupFunction, typename LoopFunction>
static auto run_frame(
    AppState& app_state,
    AppState::Frame frame, 
    const std::vector<renderer::TextureGroup>& textures, 
    const SetupFunction& setup, 
    const LoopFunction& loop
){
  setup(app_state);

  for (auto texture : textures){
    texture.allocate();
  }  

  while(!window::should_close() && app_state.current_frame == frame && !app_state.should_restart_current_frame){
    const auto frame_start_time = glfwGetTime();
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);

    loop(app_state);

    window::update();
    window::delta_time = std::min(glfwGetTime() - frame_start_time, 1.0 / 30.0); //Min 30 FPS
  }

  app_state.should_restart_current_frame = false;

  for (auto texture : textures){
    texture.free();
  }
}

} //namespace mario::frames
