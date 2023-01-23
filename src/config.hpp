#pragma once

namespace config{
  static constexpr auto FontsSourceDir = "../res/fonts/";
  static constexpr auto TexturesSourceDir = "../res/textures/";
  static constexpr auto BigValue = 999999;

  static constexpr auto BlockSize = 60.f;
  static constexpr auto BlocksInRow = 20;
  static constexpr auto BlocksInColumn = 12;

  static constexpr auto MaxLevelSize = 200;

  static constexpr auto InitialWindowWidth = BlocksInRow * BlockSize;
  static constexpr auto InitialWindowHeight = BlocksInColumn * BlockSize;

  static constexpr auto RewardForQBlock = 200;
  static constexpr auto RewardForKillingGoomba = 100;
  static constexpr auto RewardForKillingFastGoomba = 300;
  static constexpr auto RewardForKillingKoopa = 100;
  static constexpr auto RewardForKillingBeetle = 100;
  static constexpr auto RewardForEatingMushroom = 1000;
  static constexpr auto RewardForEatingFireFlower = 1000;

  static constexpr auto GreenMushroomEatMessage = "+1 HP";

  static constexpr auto GoombaWalkSpeed = 2;
  static constexpr auto FastGoombaWalkSpeed = 5;
  static constexpr auto MushroomWalkSpeed = 3;
  static constexpr auto KoopaWalkSpeed = 2;
  static constexpr auto KoopaShellWalkSpeed = 8;
  static constexpr auto BeetleShellWalkSpeed = 8;
  static constexpr auto BeetleWalkSpeed = 2;
  
  static constexpr auto PlayerPositionToScroll = 9 * BlockSize;
  static constexpr auto PlayerMaxSpeedWithoutSprint = 4.f;
  static constexpr auto PlayerMaxSpeedWithSprint = 6.f;

  static constexpr auto PlayerPositionToRestartLevel = 10000.f;
}
