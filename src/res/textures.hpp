#pragma once

#include "Renderer/Texture.hpp"

#include <array>

namespace textures{
  inline auto black = Texture("black.png");

  inline auto bricks = Texture("tile/bricks.png");  
  inline auto dirt = Texture("tile/dirt.png");
  inline auto mushroom_bot = Texture("tile/mushroom_bot.png");
  inline auto mushroom_center = Texture("tile/mushroom_center.png");
  inline auto mushroom_left = Texture("tile/mushroom_left.png");
  inline auto mushroom_right = Texture("tile/mushroom_right.png");

  inline auto red_cloud_top_left = Texture("cloud/left_top.png");
  inline auto red_cloud_top_center = Texture("cloud/center_top.png");
  inline auto red_cloud_top_right = Texture("cloud/right_top.png");
  inline auto red_cloud_bottom_left = Texture("cloud/left_bottom.png");
  inline auto red_cloud_bottom_center = Texture("cloud/center_bottom.png");
  inline auto red_cloud_bottom_right = Texture("cloud/right_bottom.png");

  inline auto coin = std::array<Texture, 3>{
    Texture("coin/0.png"),
    Texture("coin/1.png"),
    Texture("coin/2.png")
  };

  inline auto q_block = std::array<Texture, 3>{
    Texture("q_block/0.png"),
    Texture("q_block/1.png"),
    Texture("q_block/2.png"),
  };

  inline auto spinning_coin = std::array<Texture, 4>{
    Texture("spinning_coin/0.png"),
    Texture("spinning_coin/1.png"),
    Texture("spinning_coin/2.png"),
    Texture("spinning_coin/3.png"),
  };

  inline auto null_block = Texture("q_block/used.png");

  inline auto mushroom_bg = Texture("mushroom_background.png");

  inline auto small_mario = Texture("player/mario/small/normal/mario.png");
  inline auto small_mario_dead = Texture("player/mario/small/normal/dead.png");
  inline auto small_mario_jumping = Texture("player/mario/small/normal/jumping.png");
  inline auto small_mario_turning = Texture("player/mario/small/normal/turning.png");

  inline auto small_mario_walk = std::array<Texture, 3>{
    Texture("player/mario/small/normal/walk1.png"),
    Texture("player/mario/small/normal/walk2.png"),
    Texture("player/mario/small/normal/walk3.png")
  };

  inline auto goomba_walk = std::array<Texture, 2>{
    Texture("entity/goomba/0.png"),
    Texture("entity/goomba/1.png")
  };

  inline auto goomba_dead = Texture("entity/goomba/dead.png");

  inline auto mini_coin = Texture("mini.png");
}
