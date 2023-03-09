#pragma once

#include <glm/glm.hpp>

namespace config{
  static constexpr auto FontsSourceDir = "../res/fonts/";
  static constexpr auto TexturesSourceDir = "../res/textures/";
  static constexpr auto BigValue = 999999;

  static constexpr auto BlockSize = 96.f;
  static constexpr auto BlocksInRow = 20;
  static constexpr auto BlocksInColumn = 12;

  static constexpr auto HorizontalLevelWidth = 200;
  static constexpr auto VerticalLevelWidth = 20;

  static constexpr auto InitialWindowWidth = BlocksInRow * BlockSize;
  static constexpr auto InitialWindowHeight = BlocksInColumn * BlockSize;

  static constexpr auto PlayerPositionToScroll = glm::vec2(9 * BlockSize, 0.f);

  static constexpr auto PlayerPositionToRestartLevel = 10000.f;
}
