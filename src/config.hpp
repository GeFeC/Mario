#pragma once

namespace config{
  static constexpr auto FontsSourceDir = "../res/fonts/";
  static constexpr auto TexturesSourceDir = "../res/textures/";

  static constexpr auto BlocksInRow = 20;
  static constexpr auto BlocksInColumn = 12;

  static constexpr auto MaxLevelSize = 120;

  static constexpr auto InitialWindowWidth = BlocksInRow * 60;
  static constexpr auto InitialWindowHeight = BlocksInColumn * 60;

  static constexpr auto RewardForQBlock = 200;
  static constexpr auto RewardForKillingGoomba = 100;

  static constexpr auto GoombaWalkSpeed = 2;
  
  static constexpr auto PlayerPositionToScroll = 9 * 60;

  static constexpr auto PlayerPositionToRestartLevel = 10000.f;
}
