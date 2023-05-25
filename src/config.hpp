#pragma once

#include <glm/glm.hpp>

namespace config{
  static constexpr auto AppTitle = "Super Mario Boss Worlds";
  static constexpr auto InitialWindowSize = glm::vec2(1000, 625);
  static constexpr auto FrameBufferSize = glm::vec2(1920, 1152);
  static constexpr auto FrameBufferAspectRatio = FrameBufferSize.x / FrameBufferSize.y;
  static constexpr auto LevelsInWorld = 6;

  static constexpr auto FontsSourceDir = "../res/fonts/";
  static constexpr auto TexturesSourceDir = "../res/textures/";
  static constexpr auto LevelsSourceDir = "../res/levels/";
}
