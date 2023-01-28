#pragma once

#include "Window.hpp"
#include "Renderer/Texture.hpp"
#include "States/AppState.hpp"
#include "Controllers/AppController.hpp"

#include <vector>

template<typename SetupFunction, typename LoopFunction>
static auto run_frame(
    AppState& app_state,
    AppState::Frame frame, 
    const std::vector<TextureAllocator>& textures, 
    const SetupFunction& setup, 
    const LoopFunction& loop
){
  for (auto texture : textures){
    texture.allocate();
  }  

  setup(app_state);

  while(!window::should_close() && app_state.current_frame == frame && !app_state.should_restart_current_frame){
    const auto frame_start_time = glfwGetTime();

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);

    loop(app_state);

    window::update();
    window::delta_time = glfwGetTime() - frame_start_time;
  }

  app_state.should_restart_current_frame = false;

  for (auto texture : textures){
    texture.free();
  }
}
