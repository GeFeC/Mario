#pragma once

#include "Renderer/Texture.hpp"

#include <array>

namespace textures{
  inline auto black = Texture("black.png");

  inline auto bricks_particles_normal = Texture("bricks_particle/normal.png");

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

  inline auto red_bush_center = Texture("bush/mush_center.png");
  inline auto red_bush_left = Texture("bush/mush_left.png");
  inline auto red_bush_right = Texture("bush/mush_right.png");

  inline auto red_hill_center = Texture("hill/mush_center.png");
  inline auto red_hill_center_dot = Texture("hill/mush_center_dot.png");
  inline auto red_hill_left = Texture("hill/mush_left.png");
  inline auto red_hill_right = Texture("hill/mush_right.png");
  inline auto red_hill_top = Texture("hill/mush_top.png");

  inline auto red_pipe_top_left = Texture("pipe/red_left_top.png");
  inline auto red_pipe_top_right = Texture("pipe/red_right_top.png");
  inline auto red_pipe_bottom_left = Texture("pipe/red_left_bot.png");
  inline auto red_pipe_bottom_right = Texture("pipe/red_right_bot.png");

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

  inline auto fire_flower = std::array<Texture, 4>{
    Texture("flower/0.png"),
    Texture("flower/1.png"),
    Texture("flower/2.png"),
    Texture("flower/3.png")
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

  inline auto big_mario = Texture("player/mario/big/normal/mario.png");
  inline auto big_mario_jumping = Texture("player/mario/big/normal/jumping.png");
  inline auto big_mario_squating = Texture("player/mario/big/normal/squating.png");
  inline auto big_mario_turning = Texture("player/mario/big/normal/turning.png");

  inline auto big_mario_walk = std::array<Texture, 3>{
    Texture("player/mario/big/normal/walk1.png"),
    Texture("player/mario/big/normal/walk2.png"),
    Texture("player/mario/big/normal/walk3.png")
  };

  inline auto fire_big_mario = Texture("player/mario/big/fire/mario.png");
  inline auto fire_big_mario_jumping = Texture("player/mario/big/fire/jumping.png");
  inline auto fire_big_mario_squating = Texture("player/mario/big/fire/squating.png");
  inline auto fire_big_mario_turning = Texture("player/mario/big/fire/turning.png");

  inline auto fire_big_mario_walk = std::array<Texture, 3>{
    Texture("player/mario/big/fire/walk1.png"),
    Texture("player/mario/big/fire/walk2.png"),
    Texture("player/mario/big/fire/walk3.png")
  };

  inline auto black_big_mario = Texture("player/mario/big/black/mario.png");
  inline auto black_big_mario_jumping = Texture("player/mario/big/black/jumping.png");
  inline auto black_big_mario_squating = Texture("player/mario/big/black/squating.png");
  inline auto black_big_mario_turning = Texture("player/mario/big/black/turning.png");

  inline auto black_big_mario_walk = std::array<Texture, 3>{
    Texture("player/mario/big/black/walk1.png"),
    Texture("player/mario/big/black/walk2.png"),
    Texture("player/mario/big/black/walk3.png")
  };

  inline auto medium_mario = Texture("player/mario/growing.png");

  inline auto fireball = std::array<Texture, 4>{
    Texture("fireball/1.png"),
    Texture("fireball/2.png"),
    Texture("fireball/3.png"),
    Texture("fireball/4.png")
  };

  inline auto explosion = std::array<Texture, 3>{
    Texture("fireball/explosion1.png"),
    Texture("fireball/explosion2.png"),
    Texture("fireball/explosion3.png")
  };

  inline auto goomba_walk = std::array<Texture, 2>{
    Texture("entity/goomba/0.png"),
    Texture("entity/goomba/1.png")
  };
  inline auto goomba_dead = Texture("entity/goomba/dead.png");

  inline auto red_goomba_walk = std::array<Texture, 2>{
    Texture("entity/goomba/red0.png"),
    Texture("entity/goomba/red1.png")
  };
  inline auto red_goomba_dead = Texture("entity/goomba/reddead.png");

  inline auto yellow_goomba_walk = std::array<Texture, 2>{
    Texture("entity/goomba/yellow0.png"),
    Texture("entity/goomba/yellow1.png")
  };
  inline auto yellow_goomba_dead = Texture("entity/goomba/yellowdead.png");

  inline auto green_koopa_walk = std::array<Texture, 2>{
    Texture("entity/koopa/green1.png"),
    Texture("entity/koopa/green2.png")
  };
  inline auto green_koopa_dead = Texture("entity/koopa/greendead.png");
  
  inline auto red_koopa_walk = std::array<Texture, 2>{
    Texture("entity/koopa/red1.png"),
    Texture("entity/koopa/red2.png")
  };
  inline auto red_koopa_dead = Texture("entity/koopa/reddead.png");

  inline auto beetle_walk = std::array<Texture, 2>{
    Texture("entity/beetle/0.png"),
    Texture("entity/beetle/1.png")
  };
  inline auto beetle_dead = Texture("entity/beetle/dead.png");

  inline auto mushroom = Texture("entity/mushroom/mushroom.png");

  inline auto mini_coin = Texture("mini.png");
}
