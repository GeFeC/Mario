#pragma once

#include "Util/Util.hpp"
#include "config.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace mario::window{

static float delta_time = 0.f;
static GLFWwindow* g_window;

static auto resize_callback(GLFWwindow*, int window_width, int window_height){
  const auto ratio = (window_width | util::as<float>) / window_height;

  auto game_screen_width = window_width;
  auto game_screen_height = window_height;

  if (ratio > config::FrameBufferAspectRatio){                                   
    game_screen_width = window_height * config::FrameBufferAspectRatio;
  }
  else{
    game_screen_height = window_width / config::FrameBufferAspectRatio; 
  }

  const auto center_x = (window_width - game_screen_width) / 2;
  const auto center_y = (window_height - game_screen_height) / 2;

  glViewport(center_x, center_y, game_screen_width, game_screen_height);
}

static auto init(){
  if (!glfwInit()){
    throw std::runtime_error("Unable to initialise glfw!");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  g_window = glfwCreateWindow(
    config::InitialWindowSize.x, 
    config::InitialWindowSize.y, 
    config::AppTitle, 
    nullptr, 
    nullptr
  );

  if (g_window == nullptr){
    throw std::runtime_error("Unable to create window!");
  }

  glfwMakeContextCurrent(g_window);
  
  //Center window:
  const auto monitor = glfwGetPrimaryMonitor();
  const auto video_mode = glfwGetVideoMode(monitor);

  const auto centered_pos = glm::vec2(
    video_mode->width / 2,
    video_mode->height / 2
  ) - config::InitialWindowSize / 2.f;

  glfwSetWindowPos(g_window, centered_pos.x, centered_pos.y);

  //Resize callback
  glfwSetFramebufferSizeCallback(g_window, resize_callback);

  //Initialise glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    throw std::runtime_error("Unable to initialise glad!");
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   
}

static auto show(){
  glfwShowWindow(g_window);
}

static auto should_close(){
  return glfwWindowShouldClose(g_window);
}

static auto update(){
  //VSync on:
  glfwSwapInterval(1);
  glfwSwapBuffers(g_window);
}

static auto is_key_pressed(int key){
  return glfwGetKey(g_window, key) == GLFW_PRESS;
}

} //namespace mario::window
